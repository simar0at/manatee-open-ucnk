//  Copyright (c) 2003-2013  Pavel Rychly

#include <finlib/fromtof.hh>
#include <cstdlib>
#include <fcntl.h>
#include "concord.hh"

using namespace std;

static const char concsignature[] = "\230ManateeConc\0";

enum {conctype_creating, conctype_ok};

/*
 * Cache schema
 *
 * ID   OFF     SIZE          TYPE        CONTENT
 * ----------------------------------------------
 * [0]  0-13    14            char        signature, see concsignature above
 * [1]  14      1             char        added_align? (0 - no, 1 - yes)
 * [2]  15      1             char        is finished? (0 - no, 1 - yes)
 * [3]  16-23   8             NumOfPos    full size
 * [4]  24-31   8             int64_t     file size
 * [5]  32-35   4             ConcIndex   concordance size         ____________________________
 * [6]  36-39   4             ConcIndex   concordance segment size                             |*2)
 * [7]  ...     [6]*2*8       ConcItem    concordance ranges                                   |
 * [8]  ...     4             ConcIndex   view size                                            |
 * [9]  ...     [8]*4         ConcIndex   view mapping        ___                              |
 * [10] ...     4             ConcIndex   collocation #1 size    |*1)                          |
 * [11] ...     [10]*2        collocitem  collocation #1 data ___|                             |
 * [12] ...     1             ConcIndex   a zero terminating colloc section                    |
 * [13] ...     2             int16_t     number of aligned corpora                    ___     |
 * [14] ...     len(cfile)+1  char        NULL terminated aligned corpus reg. filename    |    |
 * [15] ...     4             ConcIndex   aligned #1 concordance segment size             |*3) |
 * [16] ...     [6]*2*8       ConcItem    aligned #1 concordance ranges  ___              |    |
 * [17] ...     4             ConcIndex   aligned #1 collocation #1 size    |*1)          |    |
 * [18] ...     [10]*2        collocitem  aligned #1 collocation #1 data ___|             |    |
 * [19] ...     1             ConcIndex   a zero terminating aligned #1 colloc section ___|    |
 * [20] ...     4             ConcIndex   linegroup size                                       |
 * [21] ...     [20]*2        short int   linegroup data ______________________________________|
 *
 * *1) may repeat for further collocations
 * *2) may repeat for further segements, each created by a call to save()
 *     with append=true
 * *3) may repeat for further aligned corpora
 */

Concordance::Concordance (Corpus *corp, const char *filename)
    : rng (NULL), allocated (0), used (0), view (NULL), linegroup (NULL),
      rng_mutex (NULL), thread_id (NULL), query (NULL), corp (corp),
      corp_size (corp->size()), label (0), added_align (false)
{
    FILE *f = fopen (filename, "rb");
    if (!f)
        throw FileAccessError (filename, "Concordance::Concordance");
    {
        // check signature & type
        FromFile<char> ff(f);
        for (int i = 0; i < sizeof(concsignature); i++) {
            if (concsignature[i] != ff.get())
                throw ConcNotFound (filename);
        }
        added_align = ff.get() == '\1';
        if (ff.get() != conctype_ok)
            is_finished = false;
        else
            is_finished = true;
    }
    int64_t file_size;
    ConcIndex size;
    fread (&full_size, sizeof (full_size), 1, f);
    fread (&file_size, sizeof (file_size), 1, f);
    fread (&size, sizeof (size), 1, f);
    if (!size)
        return;

    int part = 0;
    while (ftell (f) < file_size) {
        fread (&allocated, sizeof (allocated), 1, f);
        rng = (ConcItem *) realloc (rng, (used + allocated)
                                          * sizeof (ConcItem));
        size_t read = fread (rng + used, sizeof (ConcItem), allocated, f);
        if (read != allocated) {
            stringstream msg;
            msg << "Concordance corrupted (part " << part << ": expected "
                << allocated << " items, got " << read << ")";
            throw FileAccessError (filename, msg.str());
        }

        // view
        ConcIndex csize;
        fread (&csize, sizeof (csize), 1, f);
        // FromFile can seek to a wrong place
        off_t after_view = ftell(f) + csize * sizeof(ConcIndex);
        if (csize && !feof(f)) {
            if (!view)
                view = new vector<ConcIndex>();
            view->reserve (csize + view->size());
            FromFile<ConcIndex> ff (f);
            while (csize--)
                view->push_back (ff.get());
        }
        fseek (f, after_view, SEEK_SET);

        // collocations
        fread (&csize, sizeof (csize), 1, f);
        int concnum = 0;
        while (csize && !feof(f)) {
            collocitem *ci = NULL;
            ConcIndex co = 0;
            if (++concnum <= colls.size()) {
                ci = colls [concnum - 1];
                co = coll_count [concnum - 1];
            }
            ci = (collocitem *) realloc (ci, (co + csize) * sizeof(collocitem));
            fread (ci + co, sizeof (collocitem), csize, f);
            if (concnum <= colls.size()) {
                colls [concnum - 1] = ci;
                coll_count [concnum - 1] = co + csize;
            } else {
                colls.push_back (ci);
                coll_count.push_back (csize);
            }
            fread (&csize, sizeof (csize), 1, f);
        }

        // aligned corpora
        int16_t al_num = 0;
        fread (&al_num, sizeof (al_num), 1, f);
        while (al_num > aligned.size())
            aligned.push_back (new CorpData());
        for (int i = 0; i < aligned.size(); i++) {
            stringstream ss;
            char c;
            while ((c = fgetc(f)) != '\0')
                ss << c;
            string corpname = ss.str();
            CorpData *cdata = aligned [i];
            if (*corpname.rbegin() == '\1') {
                cdata->added_align = true;
                corpname.erase(corpname.size() - 1);
            }
            for (int j = 0; j < corp->aligned.size(); j++) {
                if (corp->aligned [j].first == corpname) {
                    if (!corp->aligned [j].second)
                        corp->aligned [j].second = new Corpus (corpname);
                    cdata->corp = corp->aligned [j].second;
                    break;
                }
            }
            if (!cdata->corp) {
                stringstream msg;
                msg << "Corpus " << corpname << " not aligned\n";
                throw FileAccessError (filename, msg.str());
            }
            fread (&allocated, sizeof (allocated), 1, f);
            cdata->rng = (ConcItem *) realloc (cdata->rng, (used + allocated)
                                                           * sizeof (ConcItem));
            size_t read = fread (cdata->rng + used, sizeof (ConcItem), allocated, f);
            if (read != allocated) {
                stringstream msg;
                msg << "Aligned concordance " << i << " corrupted (part "
                    << part << ": expected " << allocated << " items, got "
                    << read << ")";
                throw FileAccessError (filename, msg.str());
            }
            // aligned collocations
            fread (&csize, sizeof (csize), 1, f);
            int concnum = 0;
            while (csize && !feof(f)) {
                collocitem *ci = NULL;
                ConcIndex co = 0;
                if (++concnum <= cdata->colls.size()) {
                    ci = cdata->colls [concnum - 1];
                    co = cdata->coll_count [concnum - 1];
                }
                ci = (collocitem *) realloc (ci, (co + csize) * sizeof(collocitem));
                fread (ci + co, sizeof (collocitem), csize, f);
                if (concnum <= cdata->colls.size()) {
                    cdata->colls [concnum - 1] = ci;
                    cdata->coll_count [concnum - 1] = co + csize;
                } else {
                    cdata->colls.push_back (ci);
                    cdata->coll_count.push_back (csize);
                }
                fread (&csize, sizeof (csize), 1, f);
            }
        }
        // linegroup
        fread (&csize, sizeof (csize), 1, f);
        if (csize) {
            if (!linegroup)
                linegroup = new linegroup_t();
            linegroup->reserve (csize + linegroup->size());
            FromFile<short int> ff (f);
            while (csize--)
                linegroup->push_back (ff.get());
        }
        used += read;
        part++;
    }

    if (size != used) {
        stringstream msg;
        msg << "Concordance corrupted, parts do not match whole size (parts: "
            << used << ", whole size: " << size << ")\n";
        throw FileAccessError (filename, msg.str());
    }
    fclose (f);
}


void Concordance::save (const char *filename, bool save_linegroup, bool partial,
                        bool append)
{
    if (!partial)
        sync();
    else
        lock();
    FILE *f;
    ConcIndex curr_used = used; // always use curr_used, used may change
    ConcIndex old_size = 0;
    ConcIndex added = curr_used;
    if (append) {
        f = fopen (filename, "r+b");
        if (!f)
            throw FileAccessError (filename, "Concordance::Concordance");
        fseek (f, 24, SEEK_SET);
        NumOfPos seek;
        fread (&seek, sizeof (seek), 1, f);
        fseek (f, 32, SEEK_SET);
        fread (&old_size, sizeof (old_size), 1, f);
        added = curr_used - old_size;
        fseek (f, seek, SEEK_SET);
    } else {
        f = fopen (filename, "wb");
        if (!f)
            throw FileAccessError (filename, "Concordance::Concordance");
        fwrite (concsignature, sizeof (concsignature), 1, f);
        fwrite ("\0", 1, 22, f); // rest of header just zeroes
    }

    // concordance
    fwrite (&added, sizeof (added), 1, f);
    fwrite (rng + old_size, sizeof (ConcItem), added, f);
    {
        // view
        ToFile<ConcIndex> ff (f);
        if (!view)
            ff(0);
        else {
            ff (added);
            for (vector<ConcIndex>::iterator i=view->begin() + old_size;
                 i != view->end(); i++)
                ff (*i);
        }
    }
    {
        // collocations
        for (vector<collocitem*>::iterator ci=colls.begin();
             ci != colls.end(); ci++) {
            if (*ci) {
                fwrite (&added, sizeof (added), 1, f);
                fwrite (*ci + old_size, sizeof (collocitem), added, f);
            }
        }
        ConcIndex colsize = 0;
        fwrite (&colsize, sizeof (colsize), 1, f);
    }
    int16_t al_num = aligned.size();
    fwrite (&al_num, sizeof(al_num), 1, f);
    for (int i = 0; i < aligned.size(); i++) {
        CorpData *cdata = aligned [i];
        const char *cname = cdata->corp->get_conffile();
        fwrite (cname, 1, strlen(cname), f);
        char cname_end;
        if (cdata->added_align) {
            cname_end = '\1';
            fwrite (&cname_end, 1, 1, f);
        }
        cname_end = '\0';
        fwrite (&cname_end, 1, 1, f);
        // aligned concordance
        fwrite (&added, sizeof (added), 1, f);
        fwrite (cdata->rng + old_size, sizeof (ConcItem), added, f);
        // aligned collocations
        for (vector<collocitem*>::iterator ci=cdata->colls.begin();
             ci != cdata->colls.end(); ci++) {
            if (*ci) {
                fwrite (&added, sizeof (added), 1, f);
                fwrite (*ci + old_size, sizeof (collocitem), added, f);
            }
        }
        ConcIndex colsize = 0;
        fwrite (&colsize, sizeof (colsize), 1, f);
    }
    if (save_linegroup) {
        // linegroup 
        { // array size
            ToFile<ConcIndex> ff (f);
            if (!linegroup)
                ff(0);
            else
                ff (added);
        }
        if (linegroup) {
            ToFile<short int> ff (f);
            for (linegroup_t::iterator i=linegroup->begin() + old_size;
                 i != linegroup->end(); i++)
                ff (*i);
        }
    } else {
        // empty linegroup
        ToFile<ConcIndex> ff (f);
        ff (0);
    }

    int64_t file_size = ftell (f);
    fseek (f, 14, SEEK_SET);
    char add = (added_align ? '\1' : '\0');
    fwrite (&add, 1, 1, f);
    char conctype;
    if (finished())
        conctype = conctype_ok;
    else
        conctype = conctype_creating;
    fwrite (&conctype, sizeof (conctype), 1, f);
    fwrite (&full_size, sizeof (full_size), 1, f);
    fwrite (&file_size, sizeof (file_size), 1, f);
    fwrite (&curr_used, sizeof (curr_used), 1, f);
    fclose (f);
    if (partial)
        unlock();
}

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

/*  Copyright (c) 2011-2013  Milos Jakubicek

Obtained from http://unicode.org/repos/cldr/trunk/common/supplemental/likelySubtags.xml

by the following:

grep '<likelySubtag ' likelySubtags.xml | grep -v und |
sed 's#.* to="\([^"_]\+\)_\([^"_]\+_\)\?\([^"_]\+\)"/> <!--{ \([^;]\+\).*#lc ["\4"] = "\1_\3";#'

and modified manually for Chinese and Setswana:

(*lc) ["Chinese"] = "zh_CN";
(*lc) ["Chinese Simplified"] = "zh_CN";
(*lc) ["Chinese Traditional"] = "zh_TW";
(*lc) ["Setswana"] = "tn_BW";

*/

#include <string>
#include <cstdio>
#include <cstring>

#if defined HAVE_EXT_HASH_MAP
#include <ext/hash_map>
#    if defined HAVE_GNU_CXX_NS
         namespace std {using namespace __gnu_cxx;}
#    endif
#else
#include <hash_map>
#endif

using namespace std;
struct eqstr {
    bool operator()(const char* s1, const char* s2) const {
        return strcmp(s1, s2) == 0;
    }
};
typedef hash_map <const char *, const char *, hash<const char*>, eqstr> ss_map;

ss_map* init_langcodes() {
    ss_map *lc = new ss_map;
    (*lc) ["Afar"] = "aa_ET";
    (*lc) ["Abkhazian"] = "ab_GE";
    (*lc) ["Adyghe"] = "ady_RU";
    (*lc) ["Afrikaans"] = "af_ZA";
    (*lc) ["Aghem"] = "agq_CM";
    (*lc) ["Akan"] = "ak_GH";
    (*lc) ["Amharic"] = "am_ET";
    (*lc) ["Arabic"] = "ar_EG";
    (*lc) ["Assamese"] = "as_IN";
    (*lc) ["Asu"] = "asa_TZ";
    (*lc) ["Asturian"] = "ast_ES";
    (*lc) ["Avaric"] = "av_RU";
    (*lc) ["Aymara"] = "ay_BO";
    (*lc) ["Azerbaijani"] = "az_AZ";
    (*lc) ["Azerbaijani"] = "az_IR";
    (*lc) ["Azerbaijani"] = "az_IR";
    (*lc) ["Bashkir"] = "ba_RU";
    (*lc) ["Basaa"] = "bas_CM";
    (*lc) ["Belarusian"] = "be_BY";
    (*lc) ["Bemba"] = "bem_ZM";
    (*lc) ["Bena"] = "bez_TZ";
    (*lc) ["Bulgarian"] = "bg_BG";
    (*lc) ["Bislama"] = "bi_VU";
    (*lc) ["Bambara"] = "bm_ML";
    (*lc) ["Bengali"] = "bn_BD";
    (*lc) ["Tibetan"] = "bo_CN";
    (*lc) ["Breton"] = "br_FR";
    (*lc) ["Bodo"] = "brx_IN";
    (*lc) ["Bosnian"] = "bs_BA";
    (*lc) ["Blin"] = "byn_ER";
    (*lc) ["Catalan"] = "ca_ES";
    (*lc) ["Atsam"] = "cch_NG";
    (*lc) ["Chechen"] = "ce_RU";
    (*lc) ["Cebuano"] = "ceb_PH";
    (*lc) ["Chiga"] = "cgg_UG";
    (*lc) ["Chamorro"] = "ch_GU";
    (*lc) ["Chuukese"] = "chk_FM";
    (*lc) ["Cherokee"] = "chr_US";
    (*lc) ["Czech"] = "cs_CZ";
    (*lc) ["Welsh"] = "cy_GB";
    (*lc) ["Danish"] = "da_DK";
    (*lc) ["Taita"] = "dav_KE";
    (*lc) ["German"] = "de_DE";
    (*lc) ["Zarma"] = "dje_NE";
    (*lc) ["Duala"] = "dua_CM";
    (*lc) ["Divehi"] = "dv_MV";
    (*lc) ["Jola-Fonyi"] = "dyo_SN";
    (*lc) ["Dzongkha"] = "dz_BT";
    (*lc) ["Embu"] = "ebu_KE";
    (*lc) ["Ewe"] = "ee_GH";
    (*lc) ["Efik"] = "efi_NG";
    (*lc) ["Greek"] = "el_GR";
    (*lc) ["English"] = "en_US";
    (*lc) ["Spanish"] = "es_ES";
    (*lc) ["Estonian"] = "et_EE";
    (*lc) ["Basque"] = "eu_ES";
    (*lc) ["Ewondo"] = "ewo_CM";
    (*lc) ["Persian"] = "fa_IR";
    (*lc) ["Fulah"] = "ff_SN";
    (*lc) ["Finnish"] = "fi_FI";
    (*lc) ["Filipino"] = "fil_PH";
    (*lc) ["Fijian"] = "fj_FJ";
    (*lc) ["Faroese"] = "fo_FO";
    (*lc) ["French"] = "fr_FR";
    (*lc) ["Friulian"] = "fur_IT";
    (*lc) ["Western Frisian"] = "fy_NL";
    (*lc) ["Irish"] = "ga_IE";
    (*lc) ["Ga"] = "gaa_GH";
    (*lc) ["Gagauz"] = "gag_MD";
    (*lc) ["Scottish Gaelic"] = "gd_GB";
    (*lc) ["Gilbertese"] = "gil_KI";
    (*lc) ["Galician"] = "gl_ES";
    (*lc) ["Guarani"] = "gn_PY";
    (*lc) ["Swiss German"] = "gsw_CH";
    (*lc) ["Gujarati"] = "gu_IN";
    (*lc) ["Gusii"] = "guz_KE";
    (*lc) ["Manx"] = "gv_GB";
    (*lc) ["Hausa"] = "ha_NG";
    (*lc) ["Hawaiian"] = "haw_US";
    (*lc) ["Hebrew"] = "he_IL";
    (*lc) ["Hindi"] = "hi_IN";
    (*lc) ["Hiligaynon"] = "hil_PH";
    (*lc) ["Hiri Motu"] = "ho_PG";
    (*lc) ["Croatian"] = "hr_HR";
    (*lc) ["Haitian"] = "ht_HT";
    (*lc) ["Hungarian"] = "hu_HU";
    (*lc) ["Armenian"] = "hy_AM";
    (*lc) ["Indonesian"] = "id_ID";
    (*lc) ["Igbo"] = "ig_NG";
    (*lc) ["Sichuan Yi"] = "ii_CN";
    (*lc) ["Iloko"] = "ilo_PH";
    (*lc) ["Ingush"] = "inh_RU";
    (*lc) ["Icelandic"] = "is_IS";
    (*lc) ["Italian"] = "it_IT";
    (*lc) ["Japanese"] = "ja_JP";
    (*lc) ["Machame"] = "jmc_TZ";
    (*lc) ["Javanese"] = "jv_ID";
    (*lc) ["Georgian"] = "ka_GE";
    (*lc) ["Kabyle"] = "kab_DZ";
    (*lc) ["Jju"] = "kaj_NG";
    (*lc) ["Kamba"] = "kam_KE";
    (*lc) ["Kabardian"] = "kbd_RU";
    (*lc) ["Tyap"] = "kcg_NG";
    (*lc) ["Makonde"] = "kde_TZ";
    (*lc) ["Kabuverdianu"] = "kea_CV";
    (*lc) ["Kongo"] = "kg_CD";
    (*lc) ["Khasi"] = "kha_IN";
    (*lc) ["Koyra Chiini"] = "khq_ML";
    (*lc) ["Kikuyu"] = "ki_KE";
    (*lc) ["Kuanyama"] = "kj_NA";
    (*lc) ["Kazakh"] = "kk_KZ";
    (*lc) ["Kalaallisut"] = "kl_GL";
    (*lc) ["Kalenjin"] = "kln_KE";
    (*lc) ["Khmer"] = "km_KH";
    (*lc) ["Kannada"] = "kn_IN";
    (*lc) ["Korean"] = "ko_KR";
    (*lc) ["Komi-Permyak"] = "koi_RU";
    (*lc) ["Konkani"] = "kok_IN";
    (*lc) ["Kosraean"] = "kos_FM";
    (*lc) ["Kpelle"] = "kpe_LR";
    (*lc) ["Komi-Zyrian"] = "kpv_RU";
    (*lc) ["Karachay-Balkar"] = "krc_RU";
    (*lc) ["Kashmiri"] = "ks_IN";
    (*lc) ["Shambala"] = "ksb_TZ";
    (*lc) ["Bafia"] = "ksf_CM";
    (*lc) ["Colognian"] = "ksh_DE";
    (*lc) ["Kurdish"] = "ku_IQ";
    (*lc) ["Kurdish"] = "ku_TR";
    (*lc) ["Kurdish"] = "ku_SY";
    (*lc) ["Kurdish"] = "ku_TR";
    (*lc) ["Kumyk"] = "kum_RU";
    (*lc) ["Cornish"] = "kw_GB";
    (*lc) ["Kirghiz"] = "ky_KG";
    (*lc) ["Latin"] = "la_VA";
    (*lc) ["Langi"] = "lag_TZ";
    (*lc) ["Lahnda"] = "lah_PK";
    (*lc) ["Luxembourgish"] = "lb_LU";
    (*lc) ["Lak"] = "lbe_RU";
    (*lc) ["Lezghian"] = "lez_RU";
    (*lc) ["Ganda"] = "lg_UG";
    (*lc) ["Lingala"] = "ln_CD";
    (*lc) ["Lao"] = "lo_LA";
    (*lc) ["Lithuanian"] = "lt_LT";
    (*lc) ["Luba-Katanga"] = "lu_CD";
    (*lc) ["Luba-Lulua"] = "lua_CD";
    (*lc) ["Luo"] = "luo_KE";
    (*lc) ["Luyia"] = "luy_KE";
    (*lc) ["Latvian"] = "lv_LV";
    (*lc) ["Maithili"] = "mai_IN";
    (*lc) ["Masai"] = "mas_KE";
    (*lc) ["Moksha"] = "mdf_RU";
    (*lc) ["Maguindanaon"] = "mdh_PH";
    (*lc) ["Meru"] = "mer_KE";
    (*lc) ["Morisyen"] = "mfe_MU";
    (*lc) ["Malagasy"] = "mg_MG";
    (*lc) ["Makhuwa-Meetto"] = "mgh_MZ";
    (*lc) ["Marshallese"] = "mh_MH";
    (*lc) ["Maori"] = "mi_NZ";
    (*lc) ["Macedonian"] = "mk_MK";
    (*lc) ["Malayalam"] = "ml_IN";
    (*lc) ["Mongolian"] = "mn_MN";
    (*lc) ["Mongolian"] = "mn_CN";
    (*lc) ["Mongolian"] = "mn_CN";
    (*lc) ["Marathi"] = "mr_IN";
    (*lc) ["Malay"] = "ms_MY";
    (*lc) ["Maltese"] = "mt_MT";
    (*lc) ["Burmese"] = "my_MM";
    (*lc) ["Erzya"] = "myv_RU";
    (*lc) ["Nauru"] = "na_NR";
    (*lc) ["Nama"] = "naq_NA";
    (*lc) ["Norwegian Bokmål"] = "nb_NO";
    (*lc) ["North Ndebele"] = "nd_ZW";
    (*lc) ["Low German"] = "nds_DE";
    (*lc) ["Nepali"] = "ne_NP";
    (*lc) ["Niuean"] = "niu_NU";
    (*lc) ["Dutch"] = "nl_NL";
    (*lc) ["Kwasio"] = "nmg_CM";
    (*lc) ["Norwegian Nynorsk"] = "nn_NO";
    (*lc) ["South Ndebele"] = "nr_ZA";
    (*lc) ["Northern Sotho"] = "nso_ZA";
    (*lc) ["Nuer"] = "nus_SD";
    (*lc) ["Nyanja"] = "ny_MW";
    (*lc) ["Nyankole"] = "nyn_UG";
    (*lc) ["Occitan"] = "oc_FR";
    (*lc) ["Oromo"] = "om_ET";
    (*lc) ["Oriya"] = "or_IN";
    (*lc) ["Ossetic"] = "os_GE";
    (*lc) ["Punjabi"] = "pa_IN";
    (*lc) ["Punjabi"] = "pa_PK";
    (*lc) ["Punjabi"] = "pa_PK";
    (*lc) ["Pangasinan"] = "pag_PH";
    (*lc) ["Papiamento"] = "pap_AN";
    (*lc) ["Palauan"] = "pau_PW";
    (*lc) ["Polish"] = "pl_PL";
    (*lc) ["Pohnpeian"] = "pon_FM";
    (*lc) ["Pashto"] = "ps_AF";
    (*lc) ["Portuguese"] = "pt_BR";
    (*lc) ["Quechua"] = "qu_PE";
    (*lc) ["Romansh"] = "rm_CH";
    (*lc) ["Romanian"] = "ro_RO";
    (*lc) ["Rombo"] = "rof_TZ";
    (*lc) ["Russian"] = "ru_RU";
    (*lc) ["Kinyarwanda"] = "rw_RW";
    (*lc) ["Rwa"] = "rwk_TZ";
    (*lc) ["Sanskrit"] = "sa_IN";
    (*lc) ["Sakha"] = "sah_RU";
    (*lc) ["Samburu"] = "saq_KE";
    (*lc) ["Santali"] = "sat_IN";
    (*lc) ["Sangu"] = "sbp_TZ";
    (*lc) ["Sindhi"] = "sd_IN";
    (*lc) ["Northern Sami"] = "se_NO";
    (*lc) ["Sena"] = "seh_MZ";
    (*lc) ["Koyraboro Senni"] = "ses_ML";
    (*lc) ["Sango"] = "sg_CF";
    (*lc) ["Tachelhit"] = "shi_MA";
    (*lc) ["Sinhala"] = "si_LK";
    (*lc) ["Sidamo"] = "sid_ET";
    (*lc) ["Slovak"] = "sk_SK";
    (*lc) ["Slovenian"] = "sl_SI";
    (*lc) ["Samoan"] = "sm_WS";
    (*lc) ["Shona"] = "sn_ZW";
    (*lc) ["Somali"] = "so_SO";
    (*lc) ["Albanian"] = "sq_AL";
    (*lc) ["Serbian"] = "sr_RS";
    (*lc) ["Serbian"] = "sr_RS";
    (*lc) ["Serbian"] = "sr_ME";
    (*lc) ["Setswana"] = "tn_BW";
    (*lc) ["Swati"] = "ss_ZA";
    (*lc) ["Saho"] = "ssy_ER";
    (*lc) ["Southern Sotho"] = "st_ZA";
    (*lc) ["Swedish"] = "sv_SE";
    (*lc) ["Swahili"] = "sw_TZ";
    (*lc) ["Congo Swahili"] = "swc_CD";
    (*lc) ["Tamil"] = "ta_IN";
    (*lc) ["Telugu"] = "te_IN";
    (*lc) ["Teso"] = "teo_UG";
    (*lc) ["Tetum"] = "tet_TL";
    (*lc) ["Tajik"] = "tg_TJ";
    (*lc) ["Thai"] = "th_TH";
    (*lc) ["Tigrinya"] = "ti_ET";
    (*lc) ["Tigre"] = "tig_ER";
    (*lc) ["Turkmen"] = "tk_TM";
    (*lc) ["Tokelau"] = "tkl_TK";
    (*lc) ["Tswana"] = "tn_ZA";
    (*lc) ["Tonga"] = "to_TO";
    (*lc) ["Tok Pisin"] = "tpi_PG";
    (*lc) ["Turkish"] = "tr_TR";
    (*lc) ["Taroko"] = "trv_TW";
    (*lc) ["Tsonga"] = "ts_ZA";
    (*lc) ["Tausug"] = "tsg_PH";
    (*lc) ["Tatar"] = "tt_RU";
    (*lc) ["Tuvalu"] = "tvl_TV";
    (*lc) ["Tasawaq"] = "twq_NE";
    (*lc) ["Tahitian"] = "ty_PF";
    (*lc) ["Tuvinian"] = "tyv_RU";
    (*lc) ["Central Morocco Tamazight"] = "tzm_MA";
    (*lc) ["Udmurt"] = "udm_RU";
    (*lc) ["Uighur"] = "ug_CN";
    (*lc) ["Ukrainian"] = "uk_UA";
    (*lc) ["Ulithian"] = "uli_FM";
    (*lc) ["Urdu"] = "ur_PK";
    (*lc) ["Uzbek"] = "uz_UZ";
    (*lc) ["Uzbek"] = "uz_AF";
    (*lc) ["Uzbek"] = "uz_AF";
    (*lc) ["Vai"] = "vai_LR";
    (*lc) ["Venda"] = "ve_ZA";
    (*lc) ["Vietnamese"] = "vi_VN";
    (*lc) ["Vunjo"] = "vun_TZ";
    (*lc) ["Walser"] = "wae_CH";
    (*lc) ["Walamo"] = "wal_ET";
    (*lc) ["Waray"] = "war_PH";
    (*lc) ["Wolof"] = "wo_SN";
    (*lc) ["Xhosa"] = "xh_ZA";
    (*lc) ["Soga"] = "xog_UG";
    (*lc) ["Yapese"] = "yap_FM";
    (*lc) ["Yangben"] = "yav_CM";
    (*lc) ["Yoruba"] = "yo_NG";
    (*lc) ["Zhuang"] = "za_CN";
    (*lc) ["Chinese"] = "zh_CN";
    (*lc) ["Chinese Simplified"] = "zh_CN";
    (*lc) ["Chinese Traditional"] = "zh_TW";
    (*lc) ["Zulu"] = "zu_ZA";
    return lc;
}

static ss_map *langcodes = init_langcodes();

// vim: ts=4 sw=4 sta et sts=4 si cindent tw=80:

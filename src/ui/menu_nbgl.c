#ifdef HAVE_NBGL

#include "os.h"
#include "nbgl_content.h"
#include "nbgl_use_case.h"
#include "menu.h"

//  -----------------------------------------------------------
//  --------------------- SETTINGS MENU -----------------------
//  -----------------------------------------------------------
#define SETTING_INFO_NB 2
static const char* const INFO_TYPES[SETTING_INFO_NB] = {"Version", "Developer"};
static const char* const INFO_CONTENTS[SETTING_INFO_NB] = {APPVERSION, "Blooo"};
static const nbgl_contentInfoList_t infoList = {
    .nbInfos = SETTING_INFO_NB,
    .infoTypes = INFO_TYPES,
    .infoContents = INFO_CONTENTS,
};

//  -----------------------------------------------------------
//  ----------------------- HOME PAGE -------------------------
//  -----------------------------------------------------------
void app_quit(void) {
    // exit app here
    os_sched_exit(-1);
}
#define VENOM_VARIANT     1
#define EVERSCALE_VARIANT 2

// Define the icon based on the variant id
#if defined(VARIANT_ID) && VARIANT_ID == VENOM_VARIANT
struct nbgl_icon_details_s icon = C_app_venom_64px;
#elif defined(VARIANT_ID) && VARIANT_ID == EVERSCALE_VARIANT
struct nbgl_icon_details_s icon = C_app_everscale_64px;
#else
#error "Unsupported VARIANT_ID value"
#endif

void ui_main_menu(void) {
    nbgl_useCaseHomeAndSettings(APPNAME,
                                &icon,
                                NULL,
                                INIT_HOME_PAGE,
                                NULL,
                                &infoList,
                                NULL,
                                app_quit);
}

#endif
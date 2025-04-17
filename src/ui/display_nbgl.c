#ifdef HAVE_NBGL
#include "display.h"
#include "contract.h"
#include "ui/action/validate.h"
#include "ui/menu.h"
#include "nbgl_use_case.h"

static nbgl_contentTagValue_t pairs[10];
static nbgl_contentTagValueList_t pairList;

#define VENOM_VARIANT     1
#define EVERSCALE_VARIANT 2

// Define the icon based on the variant id
#if defined(VARIANT_ID) && VARIANT_ID == VENOM_VARIANT
static uint8_t variant_id = VENOM_VARIANT;
#elif defined(VARIANT_ID) && VARIANT_ID == EVERSCALE_VARIANT
static uint8_t variant_id = EVERSCALE_VARIANT;
#else
#error "Unsupported VARIANT_ID value"
#endif

static void review_choice_pubkey(bool choice) {
    // Answer, display a status page and go back to main
    validate_pubkey(choice);
    if (choice) {
        nbgl_useCaseReviewStatus(STATUS_TYPE_ADDRESS_VERIFIED, ui_main_menu);
    } else {
        nbgl_useCaseReviewStatus(STATUS_TYPE_ADDRESS_REJECTED, ui_main_menu);
    }
}

static void review_choice_address(bool choice) {
    // Answer, display a status page and go back to main
    validate_address(choice);
    if (choice) {
        nbgl_useCaseReviewStatus(STATUS_TYPE_ADDRESS_VERIFIED, ui_main_menu);
    } else {
        nbgl_useCaseReviewStatus(STATUS_TYPE_ADDRESS_REJECTED, ui_main_menu);
    }
}

static void review_choice_transaction(bool choice) {
    // Answer, display a status page and go back to main
    validate_transaction(choice);
    if (choice) {
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_SIGNED, ui_main_menu);
    } else {
        nbgl_useCaseReviewStatus(STATUS_TYPE_TRANSACTION_REJECTED, ui_main_menu);
    }
}

static void review_choice_message(bool choice) {
    // Answer, display a status page and go back to main
    validate_message(choice);
    if (choice) {
        nbgl_useCaseReviewStatus(STATUS_TYPE_MESSAGE_SIGNED, ui_main_menu);
    } else {
        nbgl_useCaseReviewStatus(STATUS_TYPE_MESSAGE_REJECTED, ui_main_menu);
    }
}

// TODO: Implement this
void ui_display_address() {
    // Icon
    struct nbgl_icon_details_s icon;
    if (variant_id == VENOM_VARIANT) {
#if defined(C_app_venom_40px)
        icon = C_app_venom_40px;
#endif
    } else if (variant_id == EVERSCALE_VARIANT) {
#if defined(C_app_everscale_40px)
        icon = C_app_everscale_40px;
#endif
    }
    nbgl_useCaseAddressReview(data_context.addr_context.address_str,
                              NULL,
                              &icon,
                              "Verify Address",
                              NULL,
                              review_choice_address);
}

// TODO: Implement this
void ui_display_public_key() {
    // Create the page content

    uint8_t pairIndex = 0;
    pairs[pairIndex].item = "Public Key";
    pairs[pairIndex].value = (char*) data_context.pk_context.public_key_str;
    pairIndex++;

    nbgl_contentTagValueList_t content;
    content.nbPairs = pairIndex;
    content.pairs = pairs;
    content.smallCaseForValue = false;
    content.nbMaxLinesForValue = 0;
    content.startIndex = 0;

    // Icon
    struct nbgl_icon_details_s icon;
    if (variant_id == VENOM_VARIANT) {
#if defined(C_app_venom_40px)
        icon = C_app_venom_40px;
#endif
    } else if (variant_id == EVERSCALE_VARIANT) {
#if defined(C_app_everscale_40px)
        icon = C_app_everscale_40px;
#endif
    }

    // to signing screens.
    // Setup the review screen
    nbgl_useCaseReviewLight(TYPE_OPERATION,
                            &content,
                            &icon,
                            "Verify Public Key",
                            NULL,  // No subtitle
                            "Approve",
                            review_choice_pubkey);

    // nbgl_useCaseAddressReview(data_context.pk_context.public_key_str,
    //                           NULL,
    //                           &C_app_everscale_40px,
    //                           "Verify Public Key",
    //                           NULL,
    //                           review_choice_pubkey);
}

void ui_display_sign_transaction(int flow) {
    uint8_t pairIndex = 0;

    // Action line
    const char* action = NULL;
    switch (flow) {
        case SIGN_TRANSACTION_FLOW_TRANSFER:
            action = "Transfer";
            break;
        case SIGN_TRANSACTION_FLOW_DEPLOY:
            action = "Deploy";
            break;
        case SIGN_TRANSACTION_FLOW_CONFIRM:
            action = "Confirm";
            break;
        case SIGN_TRANSACTION_FLOW_BURN:
            action = "Burn";
            break;
        default:
            THROW(ERR_INVALID_REQUEST);
            break;
    }
    pairs[pairIndex].item = "Action";
    pairs[pairIndex].value = action;
    pairIndex++;

    // Amount line
    if (strcmp(action, "Transfer") == 0 || strcmp(action, "Burn") == 0) {
        pairs[pairIndex].item = "Amount";
        pairs[pairIndex].value = (char*) data_context.sign_tr_context.amount_str;
        pairIndex++;
    }

    // Address line
    if (strcmp(action, "Transfer") == 0 || strcmp(action, "Deploy") == 0) {
        pairs[pairIndex].item = "Address";
        pairs[pairIndex].value = (char*) data_context.sign_tr_context.address_str;
        pairIndex++;
    }

    // Transaction id line
    if (strcmp(action, "Confirm") == 0) {
        pairs[pairIndex].item = "Transaction id";
        pairs[pairIndex].value = (char*) data_context.sign_tr_context.transaction_id_str;
        pairIndex++;
    }

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs = pairIndex;
    pairList.pairs = pairs;

    // Icon
    struct nbgl_icon_details_s icon;
    if (variant_id == VENOM_VARIANT) {
#if defined(C_app_venom_40px)
        icon = C_app_venom_40px;
#endif
    } else if (variant_id == EVERSCALE_VARIANT) {
#if defined(C_app_everscale_40px)
        icon = C_app_everscale_40px;
#endif
    }

    // to signing screens.
    // Setup the review screen
    nbgl_useCaseReview(TYPE_TRANSACTION,
                       &pairList,
                       &icon,
                       "Review transaction",
                       NULL,  // No subtitle
                       "Review transaction",
                       review_choice_transaction);
}

void ui_display_sign() {
    uint8_t pairIndex = 0;

    pairs[pairIndex].item = "Message";
    pairs[pairIndex].value = data_context.sign_context.to_sign_str;
    pairIndex++;

    // Setup list
    pairList.nbMaxLinesForValue = 0;
    pairList.nbPairs = pairIndex;
    pairList.pairs = pairs;

    // Icon
    struct nbgl_icon_details_s icon;
    if (variant_id == VENOM_VARIANT) {
#if defined(C_app_venom_40px)
        icon = C_app_venom_40px;
#endif
    } else if (variant_id == EVERSCALE_VARIANT) {
#if defined(C_app_everscale_40px)
        icon = C_app_everscale_40px;
#endif
    }

    // to signing screens.
    // Setup the review screen
    nbgl_useCaseReview(TYPE_MESSAGE,
                       &pairList,
                       &icon,
                       "Sign message",
                       NULL,  // No subtitle
                       "Sign message.",
                       review_choice_message);
}

#endif
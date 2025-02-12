import pytest

from ragger.backend.interface import BackendInterface
from ragger.error import ExceptionRAPDU
from ragger.firmware import Firmware
from ragger.navigator import Navigator, NavInsID
from ragger.navigator.navigation_scenario import NavigateWithScenario

from application_client.everscale_command_sender import EverscaleCommandSender
from application_client.everscale_response_unpacker import unpack_sign_tx_response
from utils import navigate_until_text_and_compare

# In this tests we check the behavior of the device when asked to sign a transaction


# In this test we send to the device a transaction to sign and validate it on screen
# The transaction is short and will be sent in one chunk
# We will ensure that the displayed information is correct by using screenshots comparison

# TODO: Add a valid raw transaction and a valid expected signature
@pytest.mark.active_test_scope
def test_sign_tx_transfer(backend: BackendInterface, navigator: Navigator, default_screenshot_path: str, test_name: str) -> None:
    # Use the app interface instead of raw interface
    client = EverscaleCommandSender(backend)

    # Raw transaction
    transaction = bytes.fromhex("00000000010904455645520001010301006c000161b3badb535d1b88d0e4d60d316567b1448568efafdf21846ecd0ba02e3adabf97000000ca7cfb9642b3d6449ea677323640010165801be2256b3d704f24c46aea3298c1a5ea8f8d1aa86ccc89474bc0570265e7898ac0000000000000000036d36956f8b969d038020000")


    # Send the sign device instruction.
    # As it requires on-screen validation, the function is asynchronous.
    # It will yield the result when the navigation is done
    with client.sign_tx(transaction):
        # Validate the on-screen request by performing the navigation appropriate for this device
        navigate_until_text_and_compare(
            backend.firmware,
            navigator,
            "Accept",
            default_screenshot_path,
            test_name
        )

    # The device as yielded the result, parse it and ensure that the signature is correct
    response = client.get_async_response().data
    _, der_sig, _ = unpack_sign_tx_response(response)
    assert der_sig.hex() == "a0396cd952160f068e0a7d6279ba2b61a2215a4dd997fcc1fe8905722341a20a86424dfdb2598b86855e73e47a1804023ff3f9afffd91825df0f58825dabd808"


# # In this test we send to the device a transaction to trig a blind-signing flow
# # The transaction is short and will be sent in one chunk
# # We will ensure that the displayed information is correct by using screenshots comparison
# def test_sign_tx_short_tx_blind_sign(firmware: Firmware,
#                                      backend: BackendInterface,
#                                      navigator: Navigator,
#                                      scenario_navigator: NavigateWithScenario,
#                                      test_name: str,
#                                      default_screenshot_path: str) -> None:
#     # Use the app interface instead of raw interface
#     client = EverscaleCommandSender(backend)
#     # The path used for this entire test
#     path: str = "m/44'/1'/0'/0/0"

#     # First we need to get the public key of the device in order to build the transaction
#     rapdu = client.get_public_key(path=path)
#     _, public_key, _, _ = unpack_get_public_key_response(rapdu.data)

#     # Create the transaction that will be sent to the device for signing
#     transaction = Transaction(
#         nonce=1,
#         to="0x0000000000000000000000000000000000000000",
#         value=0,
#         memo="Blind-sign"
#     ).serialize()

#     # Send the sign device instruction.
#     valid_instruction = [NavInsID.RIGHT_CLICK] if firmware.is_nano else [NavInsID.USE_CASE_CHOICE_REJECT]
#     # As it requires on-screen validation, the function is asynchronous.
#     # It will yield the result when the navigation is done
#     with client.sign_tx(path=path, transaction=transaction):
#         navigator.navigate_and_compare(default_screenshot_path,
#                                         test_name+"/part1",
#                                         valid_instruction,
#                                         screen_change_after_last_instruction=False)

#         # Validate the on-screen request by performing the navigation appropriate for this device
#         scenario_navigator.review_approve()

#     # The device as yielded the result, parse it and ensure that the signature is correct
#     response = client.get_async_response().data
#     _, der_sig, _ = unpack_sign_tx_response(response)
#     assert check_signature_validity(public_key, der_sig, transaction)

# # In this test se send to the device a transaction to sign and validate it on screen
# # This test is mostly the same as the previous one but with different values.
# # In particular the long memo will force the transaction to be sent in multiple chunks
# def test_sign_tx_long_tx(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
#     # Use the app interface instead of raw interface
#     client = EverscaleCommandSender(backend)
#     path: str = "m/44'/1'/0'/0/0"

#     rapdu = client.get_public_key(path=path)
#     _, public_key, _, _ = unpack_get_public_key_response(rapdu.data)

#     transaction = Transaction(
#         nonce=1,
#         to="0xde0b295669a9fd93d5f28d9ec85e40f4cb697bae",
#         value=666,
#         memo=("This is a very long memo. "
#               "It will force the app client to send the serialized transaction to be sent in chunk. "
#               "As the maximum chunk size is 255 bytes we will make this memo greater than 255 characters. "
#               "Lorem ipsum dolor sit amet, consectetur adipiscing elit. Sed non risus. Suspendisse lectus tortor, "
#               "dignissim sit amet, adipiscing nec, ultricies sed, dolor. Cras elementum ultrices diam.")
#     ).serialize()

#     with client.sign_tx(path=path, transaction=transaction):
#         scenario_navigator.review_approve()

#     response = client.get_async_response().data
#     _, der_sig, _ = unpack_sign_tx_response(response)
#     assert check_signature_validity(public_key, der_sig, transaction)


# # Transaction signature refused test
# # The test will ask for a transaction signature that will be refused on screen
# def test_sign_tx_refused(backend: BackendInterface, scenario_navigator: NavigateWithScenario) -> None:
#     # Use the app interface instead of raw interface
#     client = EverscaleCommandSender(backend)
#     path: str = "m/44'/1'/0'/0/0"

#     transaction = Transaction(
#         nonce=1,
#         to="0xde0b295669a9fd93d5f28d9ec85e40f4cb697bae",
#         value=666,
#         memo="This transaction will be refused by the user"
#     ).serialize()

#     with pytest.raises(ExceptionRAPDU) as e:
#         with client.sign_tx(path=path, transaction=transaction):
#             scenario_navigator.review_reject()

#     # Assert that we have received a refusal
#     assert e.value.status == Errors.SW_DENY
#     assert len(e.value.data) == 0

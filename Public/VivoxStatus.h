/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, July 2019
 */

#pragma once

#include "CoreMinimal.h"

#include "VivoxCoreCommon.h"
#include "VivoxStatus.generated.h"

UENUM(BlueprintType)
enum class EVivoxStatus : uint8 {
	NoMessageAvailable                                  UMETA(DisplayName = "no message available"),
	Success                                             UMETA(DisplayName = "success"),
	AsyncOperationCanceled                              UMETA(DisplayName = "async operation cancel"),
	CaptureDeviceInUse                                  UMETA(DisplayName = "capture device in use"),
	ConnectionTerminated                                UMETA(DisplayName = "connection terminated"),
	FileOpenFailed                                      UMETA(DisplayName = "file open failed"),
	HandleReserved                                      UMETA(DisplayName = "handle reserved"),
	HandleTaken                                         UMETA(DisplayName = "handle token"),
	InternalError                                       UMETA(DisplayName = "internal error"),
	InvalidArgument                                     UMETA(DisplayName = "invalid argument"),
	InvalidFormat                                       UMETA(DisplayName = "invalid format"),
	InvalidOperation                                    UMETA(DisplayName = "invalid operation"),
	InvalidState                                        UMETA(DisplayName = "invalid state"),
	InvalidValueTypeXmlQuery                            UMETA(DisplayName = "invalid value type xml query"),
	NoMatchingXmlAttributeFound                         UMETA(DisplayName = "no matching xml attribute found"),
	NoMatchingXmlNodeFound                              UMETA(DisplayName = "no matching xml node found"),
	NoMemory                                            UMETA(DisplayName = "no memory"),
	NoMoreData                                          UMETA(DisplayName = "no more date"),
	NoXLSPConfigured                                    UMETA(DisplayName = "no xlsp configured"),
	NotSupported                                        UMETA(DisplayName = "not supported"),
	PortNotAvailable                                    UMETA(DisplayName = "port not available"),
	RtpTimeout                                          UMETA(DisplayName = "rtp timeout"),
	UnableToOpenCaptureDevice                           UMETA(DisplayName = "unable to open capture device"),
	XLSPConnectFailed                                   UMETA(DisplayName = "xlsp connect failed"),
	XmppBackEndRequired                                 UMETA(DisplayName = "xmpp back end required"),
	SipBackEndRequired									UMETA(DisplayName = "sip back end required"),
	PreloginDownloadFailed                              UMETA(DisplayName = "prelogin download failed"),
	NotLoggedIn                                         UMETA(DisplayName = "not logged in"),
	PresenceMustBeEnabled                               UMETA(DisplayName = "presence must be enabled"),
	ConnectorLimitExceeded                              UMETA(DisplayName = "connector limit exceeded"),
	TargetObjectNotRelated                              UMETA(DisplayName = "target object not related"),
	TargetObjectDoesNotExist                            UMETA(DisplayName = "target object does not exist"),
	MaxLoginsPerUserExceeded                            UMETA(DisplayName = "max logins per user exceeded"),
	RequestCanceled                                     UMETA(DisplayName = "request canceled"),
	BuddyDoesNotExist                                   UMETA(DisplayName = "buddy does not exist"),
	ChannelUriRequired                                  UMETA(DisplayName = "channel uri required"),
	TargetObjectAlreadyExists                           UMETA(DisplayName = "target object already exists"),
	InvalidCaptureDeviceForRequestedOperation           UMETA(DisplayName = "invalid capture device for requested operation"),
	InvalidCaptureDeviceSpecifier                       UMETA(DisplayName = "invalid capture device specifier"),
	InvalidRenderDeviceSpecifier                        UMETA(DisplayName = "invalid render device specifier"),
	DeviceLimitReached                                  UMETA(DisplayName = "device limit reached"),
	InvalidEventType                                    UMETA(DisplayName = "invalid event type"),
	NotInitialized                                      UMETA(DisplayName = "not initialized"),
	AlreadyInitialized                                  UMETA(DisplayName = "already initialized"),
	NotImplemented                                      UMETA(DisplayName = "not implemented"),
	Timeout                                             UMETA(DisplayName = "timeout"),
	NoAuthenticationStanzaReceived                      UMETA(DisplayName = "no authentication stanza received"),
	FailedToConnectToXmppServer                         UMETA(DisplayName = "failed to connect to xmpp server"),
	SSLNegotiationToXmppServerFailed                    UMETA(DisplayName = "ssl negotiation to xmpp server failed"),
	UserOffLineOrDoesNotExist                           UMETA(DisplayName = "user off line or does not exist"),
	CaptureDeviceInvalidated                            UMETA(DisplayName = "capture device invalidated"),
	MaxEtherChannelLimitReached							UMETA(DisplayName = "max ether chaneel limit reached"),
	HostUnknown											UMETA(DisplayName = "host unknown"),
	ChannelUriTooLong									UMETA(DisplayName = "channel uri too long"),
	UserUriTooLong										UMETA(DisplayName = "user uri too long"),
	NotUninitializedYet									UMETA(DisplayName = "not uninitialized yet"),
	CallTerminatedKick									UMETA(DisplayName = "call terminated kick"),
	CallTerminatedByServer								UMETA(DisplayName = "call terminated by server"),
	ServerRtpTimeout									UMETA(DisplayName = "server rtp timeout"),
	Deprecated											UMETA(DisplayName = "deprecated"),

	UnknownXmppError									UMETA(DisplayName = "unknown xmpp error"),
	AccessTokenAlreadyUsed                              UMETA(DisplayName = "access token already used"),
	AccessTokenExpired                                  UMETA(DisplayName = "access token expired"),
	AccessTokenInvalidSignature                         UMETA(DisplayName = "access token invalid signature"),
	AccessTokenClaimsMismatch                           UMETA(DisplayName = "access token claims mismatch"),
	AccessTokenMalformed                                UMETA(DisplayName = "access token malformed"),
	AccessTokenInternalError                            UMETA(DisplayName = "access token internal error"),
	AccessTokenServiceUnavailable                       UMETA(DisplayName = "access token service unavailable"),
	AccessTokenIssuerMismatch							UMETA(DisplayName = "access token issuer mismatch"),
	XmppErrorBadRequest									UMETA(DisplayName = "xmpp error bad request"),
	XmppInternalServerError								UMETA(DisplayName = "xmpp internal server error"),
	XmppServerErrorServiceUnavailable					UMETA(DisplayName = "xmpp server error service unavailable"),
	XmppErrorServiceUnavailable							UMETA(DisplayName = "xmpp error service unavailable"),

	XmppServerResponseMalformed							UMETA(DisplayName = "xmpp server response malformed"),
	XmppServerResponseBadSdp							UMETA(DisplayName = "xmpp server response bad sdp"),
	XmppServerResponseInviteMissing						UMETA(DisplayName = "xmpp server response invite missing"),
	XmppServerResponseChanAddMissing					UMETA(DisplayName = "xmpp server response chan add missing"),
														
	NetworkNameResolutionFailed                         UMETA(DisplayName = "network name resolution failed"),
	NetworkUnableToConnectToServer                      UMETA(DisplayName = "network unable to connect to server"),
	NetworkHttpTimeout                                  UMETA(DisplayName = "network http timeout"),
	NetworkHttpInvalidUrl                               UMETA(DisplayName = "network http invalid url"),
	NetworkHttpInvalidCertificate                       UMETA(DisplayName = "network http invalid certificate"),
	NetworkHttpConnectionReset                          UMETA(DisplayName = "network http connection reset"),
	NetworkHttpInvalidServerResponse                    UMETA(DisplayName = "network http invalid server response"),
	NetworkHttpGeneralConnectionFailure                 UMETA(DisplayName = "network http general connection failure"),

	FailedToConnectToVoiceService						UMETA(DisplayName = "failed to connect to voice service"),
	FailedToSendRequestToVoiceService					UMETA(DisplayName = "failed to send request to voice service"),
	FailedToConnectToServer								UMETA(DisplayName = "failed to connect to server"),
};

FORCEINLINE EVivoxStatus fromVivox(const VivoxCoreError& vivoxCoreError) {
	switch (vivoxCoreError) {
		case VxErrorNoMessageAvailable:
			return EVivoxStatus::NoMessageAvailable;
			break;
		case VxErrorSuccess:
			return EVivoxStatus::Success;
			break;
		case VxErrorAsyncOperationCanceled:
			return EVivoxStatus::AsyncOperationCanceled;
			break;
		case VxErrorCaptureDeviceInUse:
			return EVivoxStatus::CaptureDeviceInUse;
			break;
		case VxErrorConnectionTerminated:
			return EVivoxStatus::ConnectionTerminated;
			break;
		case VxErrorFileOpenFailed:
			return EVivoxStatus::FileOpenFailed;
			break;
		case VxErrorHandleReserved:
			return EVivoxStatus::HandleReserved;
			break;
		case VxErrorHandleTaken:
			return EVivoxStatus::HandleTaken;
			break;
		case VxErrorInternalError:
			return EVivoxStatus::InternalError;
			break;
		case VxErrorInvalidArgument:
			return EVivoxStatus::InvalidArgument;
			break;
		case VxErrorInvalidFormat:
			return EVivoxStatus::InvalidFormat;
			break;
		case VxErrorInvalidOperation:
			return EVivoxStatus::InvalidOperation;
			break;
		case VxErrorInvalidState:
			return EVivoxStatus::InvalidState;
			break;
		case VxErrorInvalidValueTypeXmlQuery:
			return EVivoxStatus::InvalidValueTypeXmlQuery;
			break;
		case VxErrorNoMatchingXmlAttributeFound:
			return EVivoxStatus::NoMatchingXmlAttributeFound;
			break;
		case VxErrorNoMatchingXmlNodeFound:
			return EVivoxStatus::NoMatchingXmlNodeFound;
			break;
		case VxErrorNoMemory:
			return EVivoxStatus::NoMemory;
			break;
		case VxErrorNoMoreData:
			return EVivoxStatus::NoMoreData;
			break;
		case VxErrorNoXLSPConfigured:
			return EVivoxStatus::NoXLSPConfigured;
			break;
		case VxErrorNotSupported:
			return EVivoxStatus::NotSupported;
			break;
		case VxErrorPortNotAvailable:
			return EVivoxStatus::PortNotAvailable;
			break;
		case VxErrorRtpTimeout:
			return EVivoxStatus::RtpTimeout;
			break;
		case VxErrorUnableToOpenCaptureDevice:
			return EVivoxStatus::UnableToOpenCaptureDevice;
			break;
		case VxErrorXLSPConnectFailed:
			return EVivoxStatus::XLSPConnectFailed;
			break;
		case VxErrorXmppBackEndRequired:
			return EVivoxStatus::XmppBackEndRequired;
			break;
		case VxErrorSipBackEndRequired:
			return EVivoxStatus::SipBackEndRequired;
			break;
		case VxErrorPreloginDownloadFailed:
			return EVivoxStatus::PreloginDownloadFailed;
			break;
		case VxErrorNotLoggedIn:
			return EVivoxStatus::NotLoggedIn;
			break;
		case VxErrorPresenceMustBeEnabled:
			return EVivoxStatus::PresenceMustBeEnabled;
			break;
		case VxErrorConnectorLimitExceeded:
			return EVivoxStatus::ConnectorLimitExceeded;
			break;
		case VxErrorTargetObjectNotRelated:
			return EVivoxStatus::TargetObjectNotRelated;
			break;
		case VxErrorTargetObjectDoesNotExist:
			return EVivoxStatus::TargetObjectDoesNotExist;
			break;
		case VxErrorMaxLoginsPerUserExceeded:
			return EVivoxStatus::MaxLoginsPerUserExceeded;
			break;
		case VxErrorRequestCanceled:
			return EVivoxStatus::RequestCanceled;
			break;
		case VxErrorBuddyDoesNotExist:
			return EVivoxStatus::BuddyDoesNotExist;
			break;
		case VxErrorChannelUriRequired:
			return EVivoxStatus::ChannelUriRequired;
			break;
		case VxErrorTargetObjectAlreadyExists:
			return EVivoxStatus::TargetObjectAlreadyExists;
			break;
		case VxErrorInvalidCaptureDeviceForRequestedOperation:
			return EVivoxStatus::InvalidCaptureDeviceForRequestedOperation;
			break;
		case VxErrorInvalidCaptureDeviceSpecifier:
			return EVivoxStatus::InvalidCaptureDeviceSpecifier;
			break;
		case VxErrorInvalidRenderDeviceSpecifier:
			return EVivoxStatus::InvalidRenderDeviceSpecifier;
			break;
		case VxErrorDeviceLimitReached:
			return EVivoxStatus::DeviceLimitReached;
			break;
		case VxErrorInvalidEventType:
			return EVivoxStatus::InvalidEventType;
			break;
		case VxErrorNotInitialized:
			return EVivoxStatus::NotInitialized;
			break;
		case VxErrorAlreadyInitialized:
			return EVivoxStatus::AlreadyInitialized;
			break;
		case VxErrorNotImplemented:
			return EVivoxStatus::NotImplemented;
			break;
		case VxErrorTimeout:
			return EVivoxStatus::Timeout;
			break;
		case VxNoAuthenticationStanzaReceived:
			return EVivoxStatus::NoAuthenticationStanzaReceived;
			break;
		case VxFailedToConnectToXmppServer:
			return EVivoxStatus::FailedToConnectToXmppServer;
			break;
		case VxSSLNegotiationToXmppServerFailed:
			return EVivoxStatus::SSLNegotiationToXmppServerFailed;
			break;
		case VxErrorUserOffLineOrDoesNotExist:
			return EVivoxStatus::UserOffLineOrDoesNotExist;
			break;
		case VxErrorCaptureDeviceInvalidated:
			return EVivoxStatus::CaptureDeviceInvalidated;
			break;
		case VxErrorMaxEtherChannelLimitReached:
			return EVivoxStatus::MaxEtherChannelLimitReached;
			break;
		case VxErrorHostUnknown:
			return EVivoxStatus::HostUnknown;
			break;
		case VxErrorChannelUriTooLong:
			return EVivoxStatus::ChannelUriTooLong;
			break;
		case VxErrorUserUriTooLong:
			return EVivoxStatus::UserUriTooLong;
			break;
		case VxErrorNotUninitializedYet:
			return EVivoxStatus::NotUninitializedYet;
			break;
		case VxErrorCallTerminatedKick:
			return EVivoxStatus::CallTerminatedKick;
			break;
		case VxErrorCallTerminatedByServer:
			return EVivoxStatus::CallTerminatedByServer;
			break;
		case VxErrorServerRtpTimeout:
			return EVivoxStatus::ServerRtpTimeout;
			break;
		case VxErrorDeprecated:
			return EVivoxStatus::Deprecated;
			break;

		case VxUnknownXmppError:
			return EVivoxStatus::UnknownXmppError;
			break;
		case VxAccessTokenAlreadyUsed:
			return EVivoxStatus::AccessTokenAlreadyUsed;
			break;
		case VxAccessTokenExpired:
			return EVivoxStatus::AccessTokenExpired;
			break;
		case VxAccessTokenInvalidSignature:
			return EVivoxStatus::AccessTokenInvalidSignature;
			break;
		case VxAccessTokenClaimsMismatch:
			return EVivoxStatus::AccessTokenClaimsMismatch;
			break;
		case VxAccessTokenMalformed:
			return EVivoxStatus::AccessTokenMalformed;
			break;
		case VxAccessTokenInternalError:
			return EVivoxStatus::AccessTokenInternalError;
			break;
		case VxAccessTokenServiceUnavailable:
			return EVivoxStatus::AccessTokenServiceUnavailable;
			break;
		case VxAccessTokenIssuerMismatch:
			return EVivoxStatus::AccessTokenIssuerMismatch;
			break;
		case VxXmppErrorBadRequest:
			return EVivoxStatus::XmppErrorBadRequest;
			break;
		case VxXmppInternalServerError:
			return EVivoxStatus::XmppInternalServerError;
			break;
		case VxXmppServerErrorServiceUnavailable:
			return EVivoxStatus::XmppServerErrorServiceUnavailable;
			break;
		case VxXmppErrorServiceUnavailable:
			return EVivoxStatus::XmppErrorServiceUnavailable;
			break;

		case VxErrorXmppServerResponseMalformed:
			return EVivoxStatus::XmppServerResponseMalformed;
			break;
		case VxErrorXmppServerResponseBadSdp:
			return EVivoxStatus::XmppServerResponseBadSdp;
			break;
		case VxErrorXmppServerResponseInviteMissing:
			return EVivoxStatus::XmppServerResponseInviteMissing;
			break;
		case VxErrorXmppServerResponseChanAddMissing:
			return EVivoxStatus::XmppServerResponseChanAddMissing;
			break;

		case VxNetworkNameResolutionFailed:
			return EVivoxStatus::NetworkNameResolutionFailed;
			break;
		case VxNetworkUnableToConnectToServer:
			return EVivoxStatus::NetworkUnableToConnectToServer;
			break;
		case VxNetworkHttpTimeout:
			return EVivoxStatus::NetworkHttpTimeout;
			break;
		case VxNetworkHttpInvalidUrl:
			return EVivoxStatus::NetworkHttpInvalidUrl;
			break;
		case VxNetworkHttpInvalidCertificate:
			return EVivoxStatus::NetworkHttpInvalidCertificate;
			break;
		case VxNetworkHttpConnectionReset:
			return EVivoxStatus::NetworkHttpConnectionReset;
			break;
		case VxNetworkHttpInvalidServerResponse:
			return EVivoxStatus::NetworkHttpInvalidServerResponse;
			break;
		case VxNetworkHttpGeneralConnectionFailure:
			return EVivoxStatus::NetworkHttpGeneralConnectionFailure;
			break;

		default:
			return EVivoxStatus::NoMessageAvailable;
	}
}
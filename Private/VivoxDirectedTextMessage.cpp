/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxDirectedTextMessage.h"

#include "IAudioDevice.h"
#include "VivoxCore.h"

#include "VivoxLoginSession.h"

#include "VivoxGlobals.h"

UVivoxDirectedTextMessage *UVivoxDirectedTextMessage::CreateVivoxDirectedTextMessage(IDirectedTextMessage *directedTextMessage) {
	UVivoxDirectedTextMessage *vivoxDirectedTextMessage = NewObject<UVivoxDirectedTextMessage>();
	vivoxDirectedTextMessage->VivoxDirectedTextMessage = directedTextMessage;

	return vivoxDirectedTextMessage;
}

void UVivoxDirectedTextMessage::BeginDestroy() {

	VivoxDirectedTextMessage = nullptr;

	this->UObject::BeginDestroy();

}

UVivoxLoginSession * UVivoxDirectedTextMessage::LoginSession() {
	return UVivoxLoginSession::CreateVivoxLoginSession((ILoginSession *)&(this->VivoxDirectedTextMessage->LoginSession()));
}

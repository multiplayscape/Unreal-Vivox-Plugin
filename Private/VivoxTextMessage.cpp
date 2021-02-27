/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#include "VivoxTextMessage.h"

#include "Presence.h"
#include "VivoxCore.h"

#include "VivoxGlobals.h"

UVivoxTextMessage *UVivoxTextMessage::CreateVivoxTextMessage(ITextMessage *textMessage) {
    UVivoxTextMessage *vivoxTextMessage = NewObject<UVivoxTextMessage>();
    vivoxTextMessage->VivoxTextMessage = textMessage;

    return vivoxTextMessage;
}

void UVivoxTextMessage::BeginDestroy() {

	VivoxTextMessage = nullptr;

	this->UObject::BeginDestroy();

}

FDateTime UVivoxTextMessage::ReceivedTime() const {
    return this->VivoxTextMessage->ReceivedTime();
}

FString UVivoxTextMessage::Message() const {
    return this->VivoxTextMessage->Message();
}

FVivoxAccountId UVivoxTextMessage::Sender() const {
    return FVivoxAccountId().fromVivox(this->VivoxTextMessage->Sender());
}

FString UVivoxTextMessage::Language() const {
    return this->VivoxTextMessage->Language();
}

FString UVivoxTextMessage::ApplicationStanzaNamespace() const {
    return this->VivoxTextMessage->ApplicationStanzaNamespace();
}

FString UVivoxTextMessage::ApplicationStanzaBody() const {
    return this->VivoxTextMessage->ApplicationStanzaBody();
}
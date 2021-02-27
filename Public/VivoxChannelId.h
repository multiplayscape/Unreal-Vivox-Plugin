/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#pragma once

#include "ChannelId.h"

#include "VivoxChannel3DProperties.h"
#include "VivoxChannelId.generated.h"

UENUM(BlueprintType)
enum class EVivoxChannelType : uint8 {
    /**
     * \brief A typical conferencing channel
     */
            NonPositional,
    /**
     * \brief A conferencing channel where users' voices are rendered with 3D positional effects. Not currently supported.
     */
            Positional,
    /**
     * \brief A conferencing channel where the user's text and audio is echoed back to that user.
     */
            Echo
};

USTRUCT(BlueprintType)

struct FVivoxChannelId {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
    FString domain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
    FString name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
    FString issuer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
    EVivoxChannelType type;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
    FVivoxChannel3DProperties properties;

public:
    bool operator==(const FVivoxChannelId &a) const {
        return (this->toVivox() == a.toVivox());
    }

    ChannelId toVivox() const {
        ChannelType vivoxType = ChannelType::Positional;
        switch (type) {
            case EVivoxChannelType::NonPositional:
                vivoxType = ChannelType::NonPositional;
                break;
            case EVivoxChannelType::Positional:
                vivoxType = ChannelType::Positional;
                break;
            case EVivoxChannelType::Echo:
                vivoxType = ChannelType::Echo;
                break;
            default:
                break;
        }
        return ChannelId(issuer, name, domain, vivoxType, properties.toVivox());
    }

    FVivoxChannelId &fromVivox(const ChannelId &vivoxChannelId) {
        this->domain = vivoxChannelId.Domain();
        this->name = vivoxChannelId.Name();
        this->issuer = vivoxChannelId.Issuer();
        switch (vivoxChannelId.Type()) {
            case ChannelType::NonPositional:
                this->type = EVivoxChannelType::NonPositional;
                break;
            case ChannelType::Positional:
                this->type = EVivoxChannelType::Positional;
                break;
            case ChannelType::Echo:
                this->type = EVivoxChannelType::Echo;
                break;
            default:
                break;
        }
        this->properties.fromVivox(vivoxChannelId.Properties());

        return *this;
    }
};

FORCEINLINE uint32 GetTypeHash(const FVivoxChannelId &b) {
    return FCrc::MemCrc_DEPRECATED(&b, sizeof(FVivoxChannelId));
}
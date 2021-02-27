/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */

#pragma once

#include "AccountId.h"

#include "VivoxAccountId.generated.h"

USTRUCT(BlueprintType)

struct FVivoxAccountId {
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
    FString domain;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
    FString name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
    FString issuer;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
    FString displayName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Vivox")
	TArray<FString> spokenLanguages;

public:
    bool operator==(const FVivoxAccountId &a) const {
        return (this->toVivox() == a.toVivox());
    }

    AccountId toVivox() const {
        return AccountId(issuer, name, domain, displayName, spokenLanguages);
    }

    FVivoxAccountId &fromVivox(const AccountId &vivoxAccountId) {
        this->domain = vivoxAccountId.Domain();
        this->name = vivoxAccountId.Name();
        this->issuer = vivoxAccountId.Issuer();
        this->displayName = vivoxAccountId.DisplayName();
		this->spokenLanguages = vivoxAccountId.SpokenLanguages();
        return *this;
    }
};

FORCEINLINE uint32 GetTypeHash(const FVivoxAccountId &b) {
    return FCrc::MemCrc_DEPRECATED(&b, sizeof(FVivoxAccountId));
}
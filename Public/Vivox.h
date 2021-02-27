/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */



#pragma once



#include "Modules/ModuleManager.h"



/**

 * \brief Unreal 4 Plugin for Vivox Voice

 */

class VIVOX_API FVivoxModule : public IModuleInterface

{

public:



    /** \brief IModuleInterface implementation  - called on Modules startup */

    void StartupModule() override;

    /** \brief IModuleInterface implementation  - called on Module shutdon */

    void ShutdownModule() override;

};


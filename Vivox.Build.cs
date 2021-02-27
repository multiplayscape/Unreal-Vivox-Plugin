/* Copyright (C) Siqi.Wu - All Rights Reserved
 * Written by Siqi.Wu <lion547016@gmail.com>, May 2019
 */


using UnrealBuildTool;



public class Vivox : ModuleRules

{

	public Vivox(ReadOnlyTargetRules Target) : base(Target)

	{



		PublicIncludePaths.AddRange(

			new string[] {

				string.Format("{0}/Public", ModuleDirectory),

			}

			);





		PrivateIncludePaths.AddRange(

			new string[] {

                string.Format("{0}/Private", ModuleDirectory),

			}

			);



        PublicDependencyModuleNames.AddRange(

                            new string[] {

                                "Core",

                                "CoreUObject",

                                "VivoxCoreLibrary",

                                "Projects",

                                "Engine",

                                "VivoxCore"

                            }

                        );



                        PrivateDependencyModuleNames.AddRange(

                            new string[] {

        				        // ... add private dependencies that you statically link with here ...

        				        "VivoxCore"

                            }

                        );



		DynamicallyLoadedModuleNames.AddRange(

			new string[]

			{

            }

			);



	    if (Target.Type != TargetRules.TargetType.Server) {

            if (Target.Platform == UnrealTargetPlatform.Win64) {

                PublicDefinitions.Add ("WITH_VIVOX=1");

            } else if (Target.Platform == UnrealTargetPlatform.Mac) {

                PublicDefinitions.Add ("WITH_VIVOX=1");

            } else if (Target.Platform == UnrealTargetPlatform.Android) {

                PublicDefinitions.Add ("WITH_VIVOX=1");

            } else if (Target.Platform == UnrealTargetPlatform.IOS) {

                PublicDefinitions.Add ("WITH_VIVOX=1");

            } else {

                PublicDefinitions.Add ("WITH_VIVOX=0");

            }

        } else {

            PublicDefinitions.Add ("WITH_VIVOX=0");

        }



        // Build

        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

    }

}


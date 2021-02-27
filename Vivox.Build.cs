/* Copyright (c) 2014-2018 by Mercer Road Corp

 *

 * Permission to use, copy, modify or distribute this software in binary or source form

 * for any purpose is allowed only under explicit prior consent in writing from Mercer Road Corp

 *

 * THE SOFTWARE IS PROVIDED "AS IS" AND MERCER ROAD CORP DISCLAIMS

 * ALL WARRANTIES WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED

 * WARRANTIES OF MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL MERCER ROAD CORP

 * BE LIABLE FOR ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL

 * DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR

 * PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS

 * ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS

 * SOFTWARE.

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


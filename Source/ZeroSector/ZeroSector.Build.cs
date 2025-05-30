// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ZeroSector : ModuleRules
{
	public ZeroSector(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] { "ZeroSector" });

		PublicDependencyModuleNames.AddRange(new string[] 
			{ 
				"Core", 
				"CoreUObject", 
				"Engine", 
				"InputCore", 
				"EnhancedInput",
				"AIModule",
                "NavigationSystem",
				"GameplayTasks",
                "SlateCore",
				"Niagara",
                "MediaAssets"
            }
		);

		PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

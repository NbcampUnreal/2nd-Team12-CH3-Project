// Copyright Epic Games, Inc. All Rights Reserved.

using System.Collections.Generic;
using System.IO;
using UnrealBuildTool;

public class SPT : ModuleRules
{
	public SPT(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(new string[] {
			Path.Combine(ModuleDirectory, "Characters"), 
			Path.Combine(ModuleDirectory, "Controllers"), 
			Path.Combine(ModuleDirectory, "GameModes"),
			Path.Combine(ModuleDirectory, "AnimInstances"),
			Path.Combine(ModuleDirectory, "GameStates"),
			Path.Combine(ModuleDirectory, "SpawnVolumes"),
            Path.Combine(ModuleDirectory, "Inventory"),
            Path.Combine(ModuleDirectory, "Inventory/ItemData"),
            Path.Combine(ModuleDirectory, "Inventory/ItemWidget"),
			Path.Combine(ModuleDirectory, "UserWidget"),
            Path.Combine(ModuleDirectory, "PatrolRoutes")
        });

        PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "UMG",
                                                            "AIModule", "NavigationSystem", "GameplayTasks"});
        PrivateDependencyModuleNames.AddRange(new string[] {  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Team15CH3Project : ModuleRules
{
	public Team15CH3Project(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{	"Core", 
			"CoreUObject",
			"Engine", 
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"NavigationSystem",
			"GameplayTasks"
		});
	}
}

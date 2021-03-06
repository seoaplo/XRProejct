// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class XRProject : ModuleRules
{
	public XRProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject",
            "InputCore", "AIModule", "NavigationSystem", "GameplayTasks"});



		//PrivateDependencyModuleNadmes.AddRange(new string[] {, "EditorStyle", "Blutility"});
		// Uncomment if you are using Slate UI
		 PrivateDependencyModuleNames.AddRange(new string[] {
				"Slate",
				"SlateCore",
				 "UMG"});
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}

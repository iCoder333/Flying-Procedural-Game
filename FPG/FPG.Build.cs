// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class FPG : ModuleRules
{
	public FPG(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "MoviePlayer", "UMG" });


        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
        //PrivateDependencyModuleNames.AddRange(new string[] { "SixDOFMovement" });
        //PublicIncludePaths.AddRange(new string[] { "SixDOFMovement/Public" });

        DynamicallyLoadedModuleNames.Add("OnlineSubsystemNull");

        if (Target.Platform == UnrealTargetPlatform.IOS)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "OnlineSubsystem" });
            DynamicallyLoadedModuleNames.Add("IOSAdvertising");
        }
        else if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PrivateDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "OnlineSubsystem" });
            PrivateDependencyModuleNames.AddRange(new string[] { "OnlineSubsystemGooglePlay", "AndroidAdvertising" });
           // DynamicallyLoadedModuleNames.Add("AndroidAdvertising");
        }
    }
}

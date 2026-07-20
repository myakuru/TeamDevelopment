using UnrealBuildTool;

public class ProjectNullEditor : ModuleRules
{
	public ProjectNullEditor(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"Slate",
			"SlateCore",
			"ToolMenus",
			"UnrealEd",
			"DeveloperSettings"
		});
	}
}

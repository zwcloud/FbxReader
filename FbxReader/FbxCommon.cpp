#include <stdio.h>
#include <Windows.h>
#include "FBXCommon.h"

#pragma region debug functions
void _DebugAssert(bool expression, const char *Text, ...)
{
	if (expression)
	{
		return;
	}
	char CaptionText[12];
	char ErrorText[2048];
	va_list valist;

	strcpy_s(CaptionText, 11, "Error");

	// Build variable text buffer
	va_start(valist, Text);
	vsprintf_s(ErrorText, 1900, Text, valist);
	va_end(valist);

	// Display the message box
	MessageBoxA(NULL, ErrorText, CaptionText, MB_OK | MB_ICONEXCLAMATION);
	OutputDebugStringA(ErrorText);
}

void _DebugAssert(bool expression, const wchar_t *Text, ...)
{
	if (expression)
	{
		return;
	}
	wchar_t CaptionText[12];
	wchar_t ErrorText[2048];
	va_list valist;

	wcscpy_s(CaptionText, 11, L"Error");

	// Build variable text buffer
	va_start(valist, Text);
	vswprintf_s(ErrorText, 2000, Text, valist);
	va_end(valist);

	// Display the message box
	MessageBoxW(NULL, ErrorText, CaptionText, MB_OK | MB_ICONEXCLAMATION);
	OutputDebugStringW(ErrorText);

	DebugBreak();
}

void DebugPrintf(const char *Text, ...)
{
	char _Text[2048];
	va_list valist;

	// Build variable text buffer
	va_start(valist, Text);
	vsprintf_s(_Text, 2000, Text, valist);
	va_end(valist);

	// write to debug console
	OutputDebugStringA(_Text);
}

void DebugPrintf(const wchar_t *Text, ...)
{
	wchar_t _Text[2048];
	va_list valist;

	// Build variable text buffer
	va_start(valist, Text);
	vswprintf_s(_Text, 2000, Text, valist);
	va_end(valist);

	// write to debug console
	OutputDebugStringW(_Text);
}

#pragma endregion

#ifdef IOS_REF
#undef  IOS_REF
#define IOS_REF (*(pManager->GetIOSettings()))
#endif

void InitializeSdkObjects(FbxManager*& pManager, FbxScene*& pScene)
{
    //The first thing to do is to create the FBX Manager which is the object allocator for almost all the classes in the SDK
    pManager = FbxManager::Create();
    if( !pManager )
    {
        DebugAssert(false, "Error: Unable to create FBX Manager!\n");
    }
	else DebugPrintf("Autodesk FBX SDK version %s\n", pManager->GetVersion());

	//Create an IOSettings object. This object holds all import/export settings.
	FbxIOSettings* ios = FbxIOSettings::Create(pManager, IOSROOT);
	pManager->SetIOSettings(ios);

	//Load plugins from the executable directory (optional)
	FbxString lPath = FbxGetApplicationDirectory();
	pManager->LoadPluginsDirectory(lPath.Buffer());

    //Create an FBX scene. This object holds most objects imported/exported from/to files.
    pScene = FbxScene::Create(pManager, "My Scene");
	if( !pScene )
    {
        DebugAssert(false, "Error: Unable to create FBX scene!\n");
    }
}

void DestroySdkObjects(FbxManager* pManager, bool pExitStatus)
{
    //Delete the FBX Manager. All the objects that have been allocated using the FBX Manager and that haven't been explicitly destroyed are also automatically destroyed.
    if( pManager ) pManager->Destroy();
	if( pExitStatus ) DebugPrintf("Program Success!\n");
}

bool LoadScene(FbxManager* pManager, FbxDocument* pScene, const char* pFilename)
{
    int lFileMajor, lFileMinor, lFileRevision;
    int lSDKMajor,  lSDKMinor,  lSDKRevision;
    //int lFileFormat = -1;
    int i, lAnimStackCount;
    bool lStatus;
    char lPassword[1024];

    // Get the file version number generate by the FBX SDK.
    FbxManager::GetFileFormatVersion(lSDKMajor, lSDKMinor, lSDKRevision);

    // Create an importer.
    FbxImporter* lImporter = FbxImporter::Create(pManager,"");

    // Initialize the importer by providing a filename.
    const bool lImportStatus = lImporter->Initialize(pFilename, -1, pManager->GetIOSettings());
    lImporter->GetFileVersion(lFileMajor, lFileMinor, lFileRevision);

    if( !lImportStatus )
    {
        FbxString error = lImporter->GetStatus().GetErrorString();
        DebugPrintf("Call to FbxImporter::Initialize() failed.\n");
        DebugPrintf("Error returned: %s file:%s \n\n", error.Buffer(), pFilename);

        if (lImporter->GetStatus().GetCode() == FbxStatus::eInvalidFileVersion)
        {
            DebugPrintf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);
            DebugPrintf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);
        }

        return false;
    }

    DebugPrintf("FBX file format version for this FBX SDK is %d.%d.%d\n", lSDKMajor, lSDKMinor, lSDKRevision);

    if (lImporter->IsFBX())
    {
        DebugPrintf("FBX file format version for file '%s' is %d.%d.%d\n\n", pFilename, lFileMajor, lFileMinor, lFileRevision);

        // From this point, it is possible to access animation stack information without
        // the expense of loading the entire file.

        DebugPrintf("Animation Stack Information\n");

        lAnimStackCount = lImporter->GetAnimStackCount();

        DebugPrintf("    Number of Animation Stacks: %d\n", lAnimStackCount);
        DebugPrintf("    Current Animation Stack: \"%s\"\n", lImporter->GetActiveAnimStackName().Buffer());
        DebugPrintf("\n");

        for(i = 0; i < lAnimStackCount; i++)
        {
            FbxTakeInfo* lTakeInfo = lImporter->GetTakeInfo(i);

            DebugPrintf("    Animation Stack %d\n", i);
            DebugPrintf("         Name: \"%s\"\n", lTakeInfo->mName.Buffer());
            DebugPrintf("         Description: \"%s\"\n", lTakeInfo->mDescription.Buffer());

            // Change the value of the import name if the animation stack should be imported 
            // under a different name.
            DebugPrintf("         Import Name: \"%s\"\n", lTakeInfo->mImportName.Buffer());

            // Set the value of the import state to false if the animation stack should be not
            // be imported. 
            DebugPrintf("         Import State: %s\n", lTakeInfo->mSelect ? "true" : "false");
            DebugPrintf("\n");
        }

        // Set the import states. By default, the import states are always set to 
        // true. The code below shows how to change these states.
        IOS_REF.SetBoolProp(IMP_FBX_MATERIAL,        true);
        IOS_REF.SetBoolProp(IMP_FBX_TEXTURE,         true);
        IOS_REF.SetBoolProp(IMP_FBX_LINK,            true);
        IOS_REF.SetBoolProp(IMP_FBX_SHAPE,           true);
        IOS_REF.SetBoolProp(IMP_FBX_GOBO,            true);
        IOS_REF.SetBoolProp(IMP_FBX_ANIMATION,       true);
        IOS_REF.SetBoolProp(IMP_FBX_GLOBAL_SETTINGS, true);
    }

    // Import the scene.
    lStatus = lImporter->Import(pScene);

    if(lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
    {
        DebugPrintf("Please enter password: ");

        lPassword[0] = '\0';

        FBXSDK_CRT_SECURE_NO_WARNING_BEGIN
        DebugAssert(false, "Notimplemented\n");
        scanf("%s", lPassword);
        FBXSDK_CRT_SECURE_NO_WARNING_END

        FbxString lString(lPassword);

        IOS_REF.SetStringProp(IMP_FBX_PASSWORD,      lString);
        IOS_REF.SetBoolProp(IMP_FBX_PASSWORD_ENABLE, true);

        lStatus = lImporter->Import(pScene);

        if(lStatus == false && lImporter->GetStatus().GetCode() == FbxStatus::ePasswordError)
        {
            DebugPrintf("\nPassword is wrong, import aborted.\n");
        }
    }

    // Destroy the importer.
    lImporter->Destroy();

    return lStatus;
}

#include "stdafx.h"

typedef int (__stdcall *REGISTER_PACKAGE)(char*);

//==============================================================================
void RegisterMMTOOLSX()
{
    HINSTANCE hOCX;
    REGISTER_PACKAGE fpRegisterPackage;

    hOCX = LoadLibrary("MMTOOLSX2.OCX");
    if (hOCX)
    {
		fpRegisterPackage = (REGISTER_PACKAGE)GetProcAddress(hOCX, "RegisterPackage");
        if (fpRegisterPackage)
        {
            fpRegisterPackage("{8CC80025-F925-11D3-818A-00E07D81BC82}");  // Basic
            fpRegisterPackage("{9CC80025-F923-13D3-828A-00E07E81BC82}");  // Wave
            fpRegisterPackage("{9CC40020-F921-13D0-628A-00E07E21BA81}");  // Visual
            fpRegisterPackage("{5CE20001-2679-23D1-228B-01F04C8BAE51}");  // AudioFX
            fpRegisterPackage("{2CE2FF21-A9C5-21D2-228F-01A01C8ABE23}");  // Filter
            fpRegisterPackage("{3CE24F25-89C2-81D8-22AF-0AA0108ABE20}");  // DSound
        }

		FreeLibrary(hOCX);
    }
}

//==============================================================================
void RegisterPackages()
{
    RegisterMMTOOLSX();
}

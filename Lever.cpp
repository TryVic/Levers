//---------------------------------------------------------------------------
#define STRICT
#include <vcl.h>
#pragma hdrstop
USERES("Lever.res");
USEFORM("unGeneral.cpp", fm);
USEUNIT("kinematics.cpp");
USEUNIT("scheme.cpp");
USEFORM("unGraphics.cpp", fmGraphics);
USEUNIT("unVariant.cpp");
USEFORM("unTotal.cpp", fm3);
//---------------------------------------------------------------------------
WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	try
	{
		Application->Initialize();
		Application->Title = "Kinematic Model";
		Application->CreateForm(__classid(Tfm), &fm);
         Application->CreateForm(__classid(TfmGraphics), &fmGraphics);
         Application->CreateForm(__classid(Tfm3), &fm3);
         Application->Run();
	}
	catch (Exception &exception)
	{
		Application->ShowException(&exception);
	}
	return 0;
}
//---------------------------------------------------------------------------

[1mdiff --git a/soundXgame/MainLoadFunctions.cpp b/soundXgame/MainLoadFunctions.cpp[m
[1mindex a306ec1..595dada 100644[m
[1m--- a/soundXgame/MainLoadFunctions.cpp[m
[1m+++ b/soundXgame/MainLoadFunctions.cpp[m
[36m@@ -102,20 +102,28 @@[m [mvoid ProjectMappe::OnLoadContent(void)[m
 	[m
 [m
 	// Local Music Cubes[m
[31m-	Vector3  CubeSpwns[22] = {[m
[31m-								Vector3(0,0,0),Vector3(0,0,0),Vector3(-45,2,45),Vector3(-15,2,50),[m
[31m-								Vector3(-40,2,15),Vector3(-22,2,31),Vector3(-10,2,10),Vector3(0,0,0),[m
[31m-								Vector3(21,2,50),Vector3(15,2,25),Vector3(37,2,30),Vector3(50,2,15),[m
[31m-								Vector3(22,2,-8),Vector3(15,2,-27),Vector3(50,2,-30),Vector3(0,0,0),[m
[31m-								Vector3(45,2,-50),Vector3(0,0,0),Vector3(50,2,40),Vector3(-30,2,-45),[m
[31m-								Vector3(-23,2,-28),Vector3(-33,2,-50)[m
[32m+[m	[32mVector3  CubeSpwns[15] = {[m
[32m+[m								[32mVector3(50,2,40),[m
[32m+[m								[32mVector3(-55,2,0),[m
[32m+[m								[32mVector3(-23,2,-28),[m
[32m+[m								[32mVector3(-33,2,-50),[m
[32m+[m								[32mVector3(-40,2,15),[m
[32m+[m								[32mVector3(-35,2,50),[m
[32m+[m								[32mVector3(-10,2,10),[m
[32m+[m								[32mVector3(45,2,-50),[m
[32m+[m								[32mVector3(21,2,50),[m
[32m+[m								[32mVector3(15,2,25),[m
[32m+[m								[32mVector3(37,2,30),[m
[32m+[m								[32mVector3(50,2,15),[m
[32m+[m								[32mVector3(22,2,-8),[m
[32m+[m								[32mVector3(15,2,-27),[m
[32m+[m								[32mVector3(50,2,-30)[m
 							};[m
 	char tempString[16];[m
 	char tempString2[32];[m
[31m-	for(int i = 1;i<22;i++)[m
[32m+[m	[32mfor(int i = 1;i<15;i++)[m
 	{[m
[31m-[m
[31m-		if(i != 1 && i != 2 && i != 3 && i != 4 && i != 8 && i != 16 && i != 18)[m
[32m+[m		[32mif(i != 99) // exclude list[m
 		{[m
 			sprintf_s(tempString, sizeof(tempString),"AUDIO_%i",i);[m
 			sprintf_s(tempString2, sizeof(tempString2),"mp3/%i-Audio.mp3",i);[m
[36m@@ -137,6 +145,7 @@[m [mvoid ProjectMappe::OnLoadContent(void)[m
 			SCENE->Object(id)->GetConnected<MusicScaler>()->sensitivity=15;[m
 			SCENE->Object(id)->GetConnected<MusicScaler>()->SetClambt(0,-1.1);[m
 			SCENE->Object(id)->GetConnected<MusicScaler>()->SetThreshold(0,0.0002f);   [m
[32m+[m			[32mSCENE->Object(id)->GetConnected<AudioEmitter>()->PlayAudio();[m
 		}[m
 	}[m
 [m
[36m@@ -146,7 +155,6 @@[m [mvoid ProjectMappe::OnLoadContent(void)[m
 	SCENE->Object("AUDIO11")->AddConnectable<MusicScaler>();[m
 	SCENE->Object("AUDIO11")->GetConnected<AudioEmitter>()->Set3Dparameter(30,200);[m
 	SCENE->Object("AUDIO11")->GetConnected<AudioEmitter>()->AudioVolume(1);[m
[31m-	SCENE->Object("AUDIO11")->GetConnected<AudioEmitter>()->PlayAudio();[m
 	SCENE->Object("AUDIO11")->move(12,0,-18);[m
 	SCENE->Object("AUDIO11")->IsGrounded(false);[m
 	SCENE->Object("AUDIO11")->AddConnectable<SmoothObjectMover>();[m

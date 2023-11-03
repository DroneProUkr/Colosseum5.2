@ECHO OFF
SET DXC="/home/someone/AirSim/UE_5.2.0/Engine/Binaries/ThirdParty/ShaderConductor/Win64/dxc.exe"
SET SPIRVCROSS="spirv-cross.exe"
IF NOT EXIST %DXC% (
	ECHO Couldn't find dxc.exe under "/home/someone/AirSim/UE_5.2.0/Engine/Binaries/ThirdParty/ShaderConductor/Win64"
	GOTO :END
)
ECHO Compiling with DXC...
%DXC% -HV 2018 -T ps_6_2 -E MainPS -spirv -fspv-target-env=vulkan1.1 -Fo BasePassPixelShader.DXC.spv BasePassPixelShader.usf
WHERE %SPIRVCROSS%
IF %ERRORLEVEL% NEQ 0 (
	ECHO spirv-cross.exe not found in Path environment variable, please build it from source https://github.com/KhronosGroup/SPIRV-Cross
	GOTO :END
)
ECHO Translating SPIRV back to glsl...
%SPIRVCROSS% --vulkan-semantics --output BasePassPixelShader.SPV.glsl BasePassPixelShader.DXC.spv
:END
PAUSE
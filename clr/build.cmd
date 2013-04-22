@echo off
set path=%path%;C:/Windows/Microsoft.NET/Framework/v4.0.30319;

echo Building project...
msbuild ReRenamer.sln /nologo /v:q /p:Configuration=Release /t:Clean
msbuild ReRenamer.sln /nologo /v:q /p:Configuration=Release /clp:ErrorsOnly

echo Done.

:end
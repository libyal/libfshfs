# Script that synchronizes the local test data

$TestsInputDirectory = "tests\input"
$TestSet = "public"
$TestFiles = "hfsplus.raw"

If (-Not (Test-Path "${TestsInputDirectory}\.fshfsinfo_fs"))
{
	New-Item -Name "${TestsInputDirectory}\.fshfsinfo_fs" -ItemType "directory" | Out-Null
	Write-Output "-H" | Out-File -Encoding ascii -FilePath "${TestsInputDirectory}\.fshfsinfo_fs\options"
}

If (-Not (Test-Path ${TestsInputDirectory}))
{
	New-Item -Name ${TestsInputDirectory} -ItemType "directory" | Out-Null
}
If (-Not (Test-Path "${TestsInputDirectory}\${TestSet}"))
{
	New-Item -Name "${TestsInputDirectory}\${TestSet}" -ItemType "directory" | Out-Null
}
ForEach ($TestFile in ${TestFiles} -split " ")
{
	$UrlTestFile = [System.Uri]::EscapeDataString("${TestFile}")
	$Url = "https://raw.githubusercontent.com/log2timeline/dfvfs/refs/heads/main/test_data/${UrlTestFile}"

	$ProgressPreference = 'SilentlyContinue'
	Invoke-WebRequest -Uri ${Url} -OutFile "${TestsInputDirectory}\${TestSet}\${TestFile}"
}

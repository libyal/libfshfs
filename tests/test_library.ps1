# Tests library functions and types.
#
# Version: 20260615

$LibraryTests = "attribute_record attributes_btree_key bit_stream btree_file btree_header btree_node btree_node_descriptor btree_node_record buffer_data_handle catalog_btree_key compressed_data_handle compression deflate directory_entry directory_record error extent extents_btree_key file_entry file_record file_system fork_descriptor huffman_tree io_handle master_directory_block name notify profiler thread_record volume_header"
$LibraryTestsWithInput = "support volume"
$OptionSets = "offset" -split " "

. .\test_functions.ps1

Function RunTest
{
	param( [string]$TestName )

	$TestBinary = "fshfs_test_${TestName}"

	$TestDescription = "${TestBinary}"
	$TestExecutable = "${TestExecutablesDirectory}\${TestBinary}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType Leaf))
	{
		WriteTestResult ${TestDescription} ${ExitIgnore}

		Return ${ExitIgnore}
	}
	$Output = Invoke-Expression ${TestExecutable}
	$Result = $global:LastExitCode

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	WriteTestResult ${TestDescription} ${Result}

	Return ${Result}
}

Function RunTestWithInput
{
	param( [string]$TestName, [string[]]$TestInput )

	$OptionSet = $TestInput[0]
	$Options = $TestInput[1]
	$TestFile = $TestInput[2]

	$TestBinary = "fshfs_test_${TestName}"
	$TestFileName = (${TestFile} -split '\\')[-2..-1] -join '\'

	$TestDescription = "${TestBinary} with input: '${TestFileName}"
	$TestExecutable = "${TestExecutablesDirectory}\${TestBinary}.exe"

	If (-Not (Test-Path -Path ${TestExecutable} -PathType Leaf))
	{
		WriteTestResult ${TestDescription} ${ExitIgnore}

		Return ${ExitIgnore}
	}
	$Output = Invoke-Expression "${TestExecutable} ${Options} ${TestFile}"
	$Result = $global:LastExitCode

	If (${Result} -ne ${ExitSuccess})
	{
		Write-Host ${Output} -foreground Red
	}
	WriteTestResult ${TestDescription} ${Result}

	Return ${Result}
}

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Error "Missing test executables directory"

	Exit ${ExitFailure}
}

$Result = ${ExitIgnore}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	# Split will return an array of a single empty string when LibraryTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$Result = RunTest ${TestName}

	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

$TestInputs = GenerateTestInputs "libfshfs" ${OptionSets}

Foreach (${TestName} in ${LibraryTestsWithInput} -split " ")
{
	# Split will return an array of a single empty string when LibraryTestsWithInput is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	ForEach ($TestInput in ${TestInputs})
	{
		$Result = RunTestWithInput ${TestName} ${TestInput}

		If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
		{
			Break
		}
	}
	If ((${Result} -ne ${ExitSuccess}) -And (${Result} -ne ${ExitIgnore}))
	{
		Break
	}
}

Exit ${Result}

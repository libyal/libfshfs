# Tests library functions and types.

$LibraryTests = "attribute_record attributes_btree_key bit_stream btree_file btree_header btree_node btree_node_descriptor btree_node_record buffer_data_handle catalog_btree_key compressed_data_handle compression deflate directory_entry directory_record error extent extents_btree_key file_entry file_record file_system fork_descriptor huffman_tree io_handle master_directory_block name notify profiler thread_record volume_header"
$LibraryTestsWithInput = "support volume"
$OptionSets = "offset" -split " "

. .\test_functions.ps1

$TestExecutablesDirectory = GetTestExecutablesDirectory

If (-Not (Test-Path ${TestExecutablesDirectory}))
{
	Write-Error "Missing test executables directory"

	Exit ${ExitFailure}
}

$Result = ${ExitSuccess}

Foreach (${TestName} in ${LibraryTests} -split " ")
{
	# Split will return an array of a single empty string when LibraryTests is empty.
	If (-Not (${TestName}))
	{
		Continue
	}
	$ResultRun = RunTestBinary ${TestExecutablesDirectory} "fshfs_test_${TestName}"

	If ((${ResultRun} -ne ${ExitSuccess}) -And (${ResultRun} -ne ${ExitIgnore}))
	{
		$Result = ${ResultRun}
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
		$ResultRun = RunTestBinaryWithInput ${TestExecutablesDirectory} "fshfs_test_${TestName}" ${TestInput}

		If ((${ResultRun} -ne ${ExitSuccess}) -And (${ResultRun} -ne ${ExitIgnore}))
		{
			$Result = ${ResultRun}
		}
	}
}

Exit ${Result}

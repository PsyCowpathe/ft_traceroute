#!/bin/bash

#========== Tests List ==========

declare -a testList=(
    "-m"
    "--g"
    "--m"
    "---m 8.8.8.8"
    "0000a0"
    "60 8.8.8.8"
    "-8.8.8.8"
    "-4 -m -1 8.8.8.8"
    "-4 -m -256 8.8.8.8"
    "-m -1 8.8.8.8"
    "-m -256 8.8.8.8"
    "-f 256 8.8.8.8"
    "-f 255 8.8.8.8"
    "-f 300 8.8.8.8"
    "-f -1 8.8.8.8"
    "-q -1 8.8.8.8"
    "-q 255 8.8.8.8"
)


#========== Expected results ==========

expectedHelpMenu="Usage:
	ft_traceroute [OPTION...] HOST [ packetlen ]

Default options:
--help			Read this help and exit

Bonus options :
-m, --m			ttl max (default = 30)
-f, --f			Start from the N hop (instead from 1)
-q, --q			Set the number of probes per each hop. Default is 3
-rdns, --rdns		Resolve IP addresses to their domain names
-z, --z			Minimal time interval between probes (default 0).
			If the value is more than 10, then it specifies a
			number in milliseconds, else it is a number of
			seconds (float point values allowed too)

Arguments:
host			The host to traceroute to
packetlen		The full packet length (default is the length of an IP
			header plus 40). Can be ignored or increased to a minimal
			allowed value"

expectedEmpty="ft_traceroute: : Temporary failure in name resolution
Cannot handle \"host\" cmdline arg \`'"

declare -a expected=(
    "ft_traceroute: Option \`-m' requires an argument.\nTry 'ft_traceroute --help' for more information."
    "ft_traceroute: Bad option \`-g'\nTry 'ft_traceroute --help' for more information."
    "ft_traceroute: Option \`-m' requires an argument.\nTry 'ft_traceroute --help' for more information."
    "ft_traceroute: ---m: Name or service not known\nCannot handle \"host\" cmdline arg \`---m'\nTry 'ft_traceroute --help' for more information."
    "ft_traceroute: 0000a0: Temporary failure in name resolution\nCannot handle \"host\" cmdline arg \`0000a0'"
    "ft_traceroute: Cannot handle \"packetlen\" cmdline arg \`8.8.8.8'\nTry 'ft_traceroute --help' for more information."
    "ft_traceroute: Bad option \`-8.8.8.8'\nTry 'ft_traceroute --help' for more information."
    "ft_traceroute: Bad option \`-4'\nTry 'ft_traceroute --help' for more information."
    "ft_traceroute: Bad option \`-4'\nTry 'ft_traceroute --help' for more information."
    "ft_traceroute: max hops cannot be more than 255"
    "ft_traceroute: max hops cannot be more than 255"
    "ft_traceroute: first hop out of range"
    "ft_traceroute: first hop out of range"
    "ft_traceroute: first hop out of range"
    "ft_traceroute: first hop out of range"
    "ft_traceroute: no more than 10 probes per hop"
    "ft_traceroute: no more than 10 probes per hop"
)

totalTests=0
totalErrors=0

#========== Function ==========

compil() {

    echo -e "Run make\n"

    silence=$(make)
    echo -e "=================================================="
}

check_outputs() {

    totalTests=$((totalTests+1))
    if silence=$(diff <(printf '%b' "$1") <(printf '%b' "$2")); then
        echo "✅ Outputs identiques"
    else
        echo -e "Args = \"$3\"\n"
        echo -e "❌ Différences détectées\n"
        printf "Your :\n" "$1"
        printf "%b" "$1"
        echo -e "\n"
        printf "Expected :\n" "$2"
        printf "%b" "$2"
        echo -e "\n"
        totalErrors=$((totalErrors+1))
    fi
}

exec_with_no_args() {

    echo -e "================= No arg test ====================\n"
    your=$(./ft_traceroute 2>&1)
    check_outputs "$your" "$expectedHelpMenu" ""
    expectedIndex=$((expectedIndex+1))
    echo "=================================================="
}

exec_with_empty_args() {

    echo -e "================ Empty arg test ==================\n"
    your=$(./ft_traceroute "" 2>&1)
    check_outputs "$your" "$expectedEmpty" ""
    expectedIndex=$((expectedIndex+1))
    echo "=================================================="
}

run_unit_tests() {

    exec_with_no_args
    exec_with_empty_args

    expectedIndex=0

    for i in "${testList[@]}"
    do
        echo -e "=================== Test $expectedIndex =======================\n"
        your=$(./ft_traceroute $i 2>&1)
        check_outputs "$your" "${expected[$expectedIndex]}" "$i"
        expectedIndex=$((expectedIndex+1))
        echo "=================================================="

    done

}

display_final_result() {

    if [ "$totalErrors" -eq "0" ]; then
        echo -e ✅ $((totalTests - totalErrors))/$totalTests Tests success
    else
        echo -e ❌ $((totalTests - totalErrors))/$totalTests Tests success
    fi
    #if totalErrors
}

#compil
run_unit_tests
display_final_result

THIS_DIR=$( cd "$( dirname "${BASH_SOURCE[0]}" )" && pwd )

#source $THIS_DIR/../../env.sh

P4C_BM_PATH=$THIS_DIR/../../P4/p4c-bmv2

P4C_BM_SCRIPT=$P4C_BM_PATH/p4c_bm/__main__.py

SWITCH_PATH=$BMV2_PATH/targets/simple_switch/simple_switch

CLI_PATH=$BMV2_PATH/targets/simple_switch/sswitch_CLI

# Probably not very elegant but it works nice here: we enable interactive mode
# to be able to use fg. We start the switch in the background, sleep for 2
# minutes to give it time to start, then add the entries and put the switch
# process back in the foreground
set -m
$P4C_BM_SCRIPT headers_and_parser.p4 --json headers_and_parser.json  --p4-v1.1 
$P4C_BM_SCRIPT headers_simpl.p4 --json headers_simpl.json  --p4-v1.1 
$P4C_BM_SCRIPT headers_full.p4 --json headers_full.json  --p4-v1.1 
if [ $? -ne 0 ]; then
echo "p4 compilation failed"
exit 1
fi


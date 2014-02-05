#! /bin/bash

if [ -f "./bash_color_codes.var" ]; then
    source ./bash_color_codes.var
elif [ -f "./scripts/bash_color_codes.var" ]; then
    source ./scripts/bash_color_codes.var
fi

message_error()
{
    MESSAGE_STRING=${1}

    echo -e "${Cyan}[${Red}ERROR${Cyan}]${Color_Off}: ${MESSAGE_STRING}" >&2
}



message_info()
{
    MESSAGE_STRING=${1}

    echo -e "${Cyan}[${Green}INFO${Cyan}]${Color_Off}: ${MESSAGE_STRING}"
}


define pc_trigger
    set $tselect = $arg0  
    set $tdata1 = 0x280000000000105C   
    set $tdata2 = $arg1   
end

define store_trigger
    set $tselect = $arg0
    set $tdata1 = 0x280000000000105A
    set $tdata2 = $arg1
end

define get_fn_pc
    if $argc < 1
        printf "Usage: get_fn_pc <function_name>\n"
        return
    end
    set $fn_pc = (uintptr_t) $arg0
    printf "Function %s is at 0x%x\n", $arg0, $fn_pc
end
	
define trig_break
    if $argc < 2
        printf "Usage: fn_break <trigger_number(allowed values :0,1,2,3)> <function_name>\n"
        return
    end
    set $tselect = $arg0
    set $fn_pc = (uintptr_t) $arg1
    set $tdata1 = 0x280000000000105C
    set $tdata2 = $fn_pc
    printf "Trigger %x set at function pc 0x%x\n", $arg0, $fn_pc
end

define clear_trig
    set $tselect = $arg0  
    set $tdata1 = 0   
    set $tdata2 = 0   
end

define clear_step
    set $tselect = $arg0  
    set $myvar1 = $tdata1
    set $myvar2 = $tdata2
    set $tdata1 = 0   
    set $tdata2 = 0   
    si
    set $tdata1 = $myvar1
    set $tdata2 = $myvar2
end

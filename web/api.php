<?php

switch ($_REQUEST['method']) {
    case 'setting':
        switch ($_REQUEST['setting']) {
            case 'coldstart':
                $value = intval($_REQUEST['value']);
                file_put_contents('/var/run/hotpi/coldstart', $value);
                print '{ status: "ok" }';
                break;

            case 'quietmode':
                $value = intval($_REQUEST['value']);
                file_put_contents('/var/run/hotpi/quietmode', $value);
                print '{ status: "ok" }';

            case 'endcycle':
                $value = intval($_REQUEST['value']);
                file_put_contents('/var/run/hotpi/endcycle', $value);
                print '{ status: "ok" }';
                break;
        }
        break;
}

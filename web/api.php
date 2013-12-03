<?php

switch ($_REQUEST['method']) {
    case 'setting':
        switch ($_REQUEST['setting']) {
            case 'coldstart':
                $value = intval($_REQUEST['value']);
                file_put_contents('/var/run/hotpi/coldstart', $value);
                break;

            case 'quietmode':
                $value = intval($_REQUEST['value']);
                file_put_contents('/var/run/hotpi/quietmode', $value);

                print '{ status: "ok" }';
                break;
        }
        break;
}

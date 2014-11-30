<!DOCTYPE html>
<html>
<head>
<meta http-equiv="refresh" content="30">
<title>HotPi</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="http://code.jquery.com/mobile/1.3.2/jquery.mobile-1.3.2.min.css" />
<script src="http://code.jquery.com/jquery-1.9.1.min.js"></script>
<script src="http://code.jquery.com/mobile/1.3.2/jquery.mobile-1.3.2.min.js"></script>
<script src="/app.js"></script>
<style type="text/css">
.ui-table-columntoggle-btn { display: none; }
</style>
</head>
<body>
<?php
    $status = array();
    foreach (file("/var/run/hotpi/status") as $line) {
        $fields = explode(',', $line);
        $status[$fields[0]] = trim($fields[1]);
    }

	$coldstart = intval(@file_get_contents('/var/run/hotpi/coldstart'));
	$quietmode = intval(@file_get_contents('/var/run/hotpi/quietmode'));
	$endcycle = intval(@file_get_contents('/var/run/hotpi/endcycle'));
?>
<div data-role="page">
	<div data-role="header" data-theme="a" data-fullscreen="true">
		<h1>HotPi</h1>
        <a href="#panel-nav" data-icon="bars" data-iconpos="notext">Navigation</a>
	</div><!-- /header -->

	<div data-role="content">
        <h3>Environment</h3>
        <table data-role="table" id="movie-table" class="table-stroke" data-mode="columntoggle"><thead>
            <tr><th>Zone</th><th>Temperature</th><th>Demand</th></tr>
        </thead><tbody>
            <tr><th>Upstairs</th><td><?php print $status['upstairs']; ?>F</td><td>On</td></tr>
            <tr><th>Downstairs</th><td><?php print $status['downstairs']; ?>F</td><td>On</td></tr>
            <tr><th>Front Room</th><td><?php print $status['frontroom']; ?>F</td><td>On</td></tr>
        </tbody></table>

        <h3>Boiler</h3>
        <table data-role="table" id="movie-table" class="table-stroke" data-mode="columntoggle"><thead>
            <tr><th>Aspect</th><th>Status</th></tr>
        </thead><tbody>
            <tr><th>Temperature</th><td><?php print $status['boiler']; ?>F</td></tr>
            <tr><th>Return</th><td><?php print $status['return']; ?>F</td></tr>
            <tr><th>Mode</th><td><?php print $status['mode']; ?></td></tr>
            <tr><th>Devices</th><td><?php print $status['devices']; ?></td></tr>
        </tbody></table>

	</div><!-- /content -->


	<div data-role="footer">
		<h4>System Status</h4>
	</div><!-- /footer -->

    <div data-role="panel" id="panel-nav" class="jqm-nav-panel" data-position="left" data-display="reveal" data-theme="a">
        <div class="ui-panel-inner">
            <h3>Settings</h3>
            <form>
                <fieldset data-role="controlgroup">
                    <!--legend>Settings:</legend>-->
                    <input type="checkbox" name="checkbox-quiet" id="checkbox-quiet" <?php print ($quietmode) ? 'checked="checked"' : ''?>>
                    <label for="checkbox-quiet">Quiet Mode</label>
                    <input type="checkbox" name="checkbox-coldstart" id="checkbox-coldstart" <?php print ($coldstart) ? 'checked="checked"' : ''?>>
                    <label for="checkbox-coldstart">Cold Start</label>
                    <input type="checkbox" name="checkbox-endcycle" id="checkbox-endcycle" <?php print ($endcycle) ? 'checked="checked"' : ''?>>
                    <label for="checkbox-endcycle">End Cycle</label>
                </fieldset>
            </form>

            <!--<a href="#demo-links" data-rel="close" data-role="button" data-theme="a"
               data-icon="delete" data-inline="true" data-corners="true"
               data-shadow="true" data-iconshadow="true" data-wrapperels="span">Close panel</a>-->
        </div>

        <!--<ul data-role="listview" data-inset="false" data-filter="true" data-theme="a" data-divider-theme="a"  data-icon="false" data-filter-placeholder="Search..." class="jqm-list">
            <li><a href="../../">Home</a></li>
            <li><a href="../../guides/">Guides</a></li>
            <li><a href="../../demos/">Demos</a></li>
        </ul>-->
    </div>
</div>
</body>
</html>

<!DOCTYPE html>
<html>
<head>
<title>HotPi</title>
<meta name="viewport" content="width=device-width, initial-scale=1">
<link rel="stylesheet" href="http://code.jquery.com/mobile/1.3.2/jquery.mobile-1.3.2.min.css" />
<script src="http://code.jquery.com/jquery-1.9.1.min.js"></script>
<script src="http://code.jquery.com/mobile/1.3.2/jquery.mobile-1.3.2.min.js"></script>
<style type="text/css">
.ui-table-columntoggle-btn { display: none; }
</style>
</head>
<body>
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
            <tr><th>Upstairs</th><td>72.0F</td><td>On</td></tr>
            <tr><th>Downstairs</th><td>72.0F</td><td>On</td></tr>
            <tr><th>Front Room</th><td>72.0F</td><td>On</td></tr>
        </tbody></table>

        <h3>Boiler</h3>
        <table data-role="table" id="movie-table" class="table-stroke" data-mode="columntoggle"><thead>
            <tr><th>Aspect</th><th>Status</th></tr>
        </thead><tbody>
            <tr><th>Temperature</th><td>212.50F</td></tr>
            <tr><th>Mode</th><td>Overheat: Dumping</td></tr>
            <tr><th>Devices</th><td>Circ: ON, Draft: ON</td></tr>
        </tbody></table>

	</div><!-- /content -->


	<div data-role="footer">
		<h4>Page Footer</h4>
	</div><!-- /footer -->

    <div data-role="panel" id="panel-nav" class="jqm-nav-panel" data-position="left" data-display="reveal" data-theme="a">
        <div class="ui-panel-inner">
            <h3>Settings</h3>
            <form>
                <fieldset data-role="controlgroup">
                    <!--legend>Settings:</legend>-->
                    <input type="checkbox" name="checkbox-quiet" id="checkbox-quiet">
                    <label for="checkbox-quiet">Quiet Mode</label>
                    <input type="checkbox" name="checkbox-coldstart" id="checkbox-coldstart">
                    <label for="checkbox-coldstart">Cold Start</label>
                </fieldset>
            </form>

            <a href="#demo-links" data-rel="close" data-role="button" data-theme="a"
               data-icon="delete" data-inline="true" data-corners="true"
               data-shadow="true" data-iconshadow="true" data-wrapperels="span">Close panel</a>
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

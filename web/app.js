(function($) {

$(document).ready(function() {
    $("input[type='checkbox']").bind("change", function(e) {
        var setting = {
            method: 'setting',
            setting: '',
            value: (e.currentTarget.checked) ? 1 : 0
        };

        switch (e.currentTarget.id) {
            case 'checkbox-quiet':
                setting.setting = 'quietmode';
                $.ajax({
                    url: '/api.php',
                    data: setting,
                    timeout: 5000,
                    success: function() {
                        window.location.reload();
                    }
                });
                break;

            case 'checkbox-coldstart':
                setting.setting = 'coldstart';
                $.ajax({
                    url: '/api.php',
                    data: setting,
                    timeout: 5000,
                    success: function() {
                        window.location.reload();
                    }
                });
                break;

            case 'checkbox-endcycle':
                setting.setting = 'endcycle';
                $.ajax({
                    url: '/api.php',
                    data: setting,
                    timeout: 5000,
                    success: function() {
                        window.location.reload();
                    }
                });
                break;
        }
    });
});

})(jQuery);

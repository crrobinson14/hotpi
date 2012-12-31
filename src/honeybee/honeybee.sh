#!/bin/sh

### BEGIN INIT INFO
# Provides:        hotpi-honeybee
# Required-Start:  $network $syslog
# Required-Stop:   $network $syslog
# Default-Start:   2 3 4 5
# Default-Stop:    1
# Short-Description: Start HotPi Honeybee daemon
### END INIT INFO

PATH=/sbin:/bin:/usr/sbin:/usr/bin

. /lib/lsb/init-functions

DAEMON=/usr/sbin/hotpi-honeybee
PIDFILE=/var/run/hotpi-honeybee.pid
CONFFILE=/etc/hotpi/honeybee.conf
SERVICE="hotpi-honeybee"
SRVNAME="HotPi Honeybee"

test -x $DAEMON || exit 5

case $1 in
    start)
        log_daemon_msg "Starting $SRVNAME" "$SERVICE"
        if [ ! -f $CONFFILE ]; then
            log_failure_msg "Please create $CONFFILE first!"
            exit 1
        fi
        start-stop-daemon --start --quiet --oknodo --pidfile $PIDFILE --startas $DAEMON
        status=$?
        log_end_msg $status
        ;;
    stop)
        log_daemon_msg "Stopping $SRVNAME" "$SERVICE"
        start-stop-daemon --stop --quiet --oknodo --pidfile $PIDFILE
        log_end_msg $?
        rm -f $PIDFILE
        ;;
    restart|force-reload)
        $0 stop && sleep 2 && $0 start
        ;;
    try-restart)
        if $0 status >/dev/null; then
            $0 restart
        else
            exit 0
        fi
        ;;
    reload)
        exit 3
        ;;
    status)
        status_of_proc $DAEMON "$SRVNAME"
        ;;
    *)
        echo "Usage: $0 {start|stop|restart|try-restart|force-reload|status}"
        exit 2
        ;;
esac

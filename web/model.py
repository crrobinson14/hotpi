import webapp2
from google.appengine.ext import ndb


class Component:
    """A component is an individual subsystem being monitored or controlled, such as a circulator."""

    # The name of the component, e.g. "Boiler Temperature"
    name = ndb.StringProperty()

    # The type of the component, e.g. "Zone" or "Boiler". Determines the program it runs.
    ctype = ndb.StringProperty()

    # The priority of the component, within its class.
    priority = ndb.IntegerProperty()

    # The state, for state-driven components, e.g. "Running" for a circulator or "DemandHeat" or "Heat" for a zone
    state = ndb.StringProperty()

    # For components that require it, the current target temperature
    target = ndb.FloatProperty()

    # For components that have targets, the ideal range around that target, in degrees
    idealrange = ndb.FloatProperty()

    # For components that have targets, the worst-case range around that target, in degrees
    maxrange = ndb.FloatProperty()


class ScheduleEntry:
    """Changes a component's configuration at a specific day/time during a week."""

    # The component this schedule entry is for
    component = ndb.KeyProperty()

    # The day of the week
    day = ndb.IntegerProperty()

    # The time of the day, in minutes
    time = ndb.IntegerProperty()

    # The priority to set
    priority = ndb.IntegerProperty()

    # The state to set
    state = ndb.StringProperty()

    # The target to set
    target = ndb.FloatProperty()

    # The ideal range to set
    idealrange = ndb.FloatProperty()

    # The max range to set
    maxrange = ndb.FloatProperty()


class DataPoint:
    """A data point is a state/value for a component at a specific moment in time."""

    # The component this data point is for
    component = ndb.KeyProperty

    # The date/time this was recorded
    recorded = ndb.DateTimeProperty(auto_now_add=True)

    # The state for the component, if applicable
    state = ndb.StringProperty()

    # The value for the component, if applicable
    value = ndb.FloatProperty()

    # The priority that was set
    priority = ndb.IntegerProperty()

    # The state to set
    state = ndb.StringProperty()

    # The target that was set
    target = ndb.FloatProperty()

    # The ideal range that was set
    idealrange = ndb.FloatProperty()

    # The max range that was set
    maxrange = ndb.FloatProperty()

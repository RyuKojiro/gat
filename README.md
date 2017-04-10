### NAME
**gat** -- GTK+ ACPI Tray Battery Monitor

### SYNOPSIS
**gat** *devname* *...*

### DESCRIPTION
**gat** is a GTK+ based utility for monitoring battery status from any window
manager that has a system tray capable of containing GTK+ system tray
icons. GTK+-3.0 was chosen because it is compatible with most window
managers, and already contains all of the necessary icon assets for all
of the battery states reflected by **gat**.

**gat** is intended to be used on machines that do not have apm, as an
alternative to other (more portable) apm-based solutions.

It is also worth noting, that due to the nature of ACPI, **gat** must poll
the status of the device, and does so once every two seconds. Charging
status is also unrelated to external power sources, but is instead only
an indication of charging versus discharging. Because of this, the tray
icon may indicate that a battery is full, and not charging, while plugged
in. This is normal, and merely indicates that no positive charging of the
battery is happening.

### EXAMPLES
Invoke **gat** with the device name you want it to monitor as the sole
argument. For example:

$ **gat** acpibat0 &

This will spawn an instance of **gat** into your system tray with the status
of that device displayed.  If you have more than one battery to monitor,
just invoke **gat** with an argument for each device.

To quit a running instance of **gat**, simply click on the tray icon and
select the ``Quit'' menu item.

### SEE ALSO
envstat(8), envsys(4)

### AUTHORS
**gat** was written by Daniel Loffgren.

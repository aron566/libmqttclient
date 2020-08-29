TEMPLATE = subdirs

SUBDIRS = mqtt mqttclient

mqttclient.depends = mqtt

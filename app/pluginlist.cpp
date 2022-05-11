#include "pluginlist.h"

PluginList::PluginList(QObject *parent) : QObject(parent)
{

    qDebug() << "Plugin list constructed";
}

PluginList::~PluginList() {
    qDebug() << "Plugin list destroyed";
}
void PluginList::initPlugins()  {
    for(PluginObject * plugin : qAsConst(m_plugins)){
        plugin->init();
    }
}

PluginObject *PluginList::addPlugin(QString pluginPath) {
    PluginObject *plugin = new PluginObject(pluginPath, this);

    connect(plugin, &PluginObject::loaded, this, &PluginList::pluginLoadedHandler);
    m_plugins.append(plugin);
    return plugin;
}
void PluginList::addPlugin(PluginObject *plugin) {
    if(plugin) {
        connect(plugin, &PluginObject::loaded, this, &PluginList::pluginLoadedHandler);
        m_plugins.append(plugin);
        int index = m_plugins.indexOf(plugin);
        emit pluginAdded(index);
    }
}

void PluginList::pluginLoadedHandler() {
    PluginObject * sender = qobject_cast<PluginObject *> (QObject::sender());
    if(sender) {
        int index = m_plugins.indexOf(sender);
        emit pluginLoaded(index);
    }
}

bool PluginList::containsPlugin(QString pluginName){
    for(PluginObject * plugin : qAsConst(m_plugins)){
        if(plugin->getName() == pluginName) {
            return true;
        }
    }
    return false;
}

PluginObject *PluginList::getPlugin(QString pluginName) {
    for(PluginObject * plugin : qAsConst(m_plugins)){
        if(plugin->getName() == pluginName) {
            return plugin;
        }
    }
    return nullptr;
}


void PluginList::handleMessage(QString id, QVariant message){
    for(PluginObject * plugin : qAsConst(m_plugins)){
        plugin->handleMessage(id, message);
    }
}

void PluginList::callSlot(QString pluginName, QString slot) {
    PluginObject * plugin = getPlugin(pluginName);
    if(plugin) {
        plugin->callSlot(slot);
    }
}
PluginObject *PluginList::at(int index){
    return m_plugins.at(index);
}
int PluginList::size(){
    return m_plugins.size();
}
int PluginList::indexOf(PluginObject * object) {
    return m_plugins.indexOf(object);
}

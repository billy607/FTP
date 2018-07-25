#ifndef LABELINGSPECIICATION_H
#define LABELINGSPECIICATION_H

#include<QMap>
#include<QString>
class LabelingEnumDef;
namespace labelingbase {

class LabelingSpeciication
{
public:
    static LabelingSpeciication* getInstance();

    QMap< int , LabelingEnumDef *> getEnumDefinitions() const{
        return enumDefinitions;
    }
    bool containsEnumDef(int key) const {
        return enumDefinitions.contains(key);
    }
    LabelingEnumDef * getDefinition(int key) const {
        Q_ASSERT(enumDefinitions.contains(key));
        return enumDefinitions.value(key);
    }
    void addEnumDefinition(int key , LabelingEnumDef * def) {
        Q_ASSERT(!containsEnumDef(key));
        enumDefinitions.insert(key,def);
    }

private:
    static LabelingSpeciication * instance;

    LabelingSpeciication(){}

private:
    QMap< int , LabelingEnumDef *> enumDefinitions;

};

}


#endif // LABELINGSPECIICATION_H

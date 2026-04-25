#ifndef ADDITEMCOMMAND_H
#define ADDITEMCOMMAND_H
#include "DiagramScene.h"
#include<QUndoCommand>
class AddItemCommand : public QUndoCommand
{
public:
    AddItemCommand(DiagramScene *scene, DiagramItem *item)
        : m_scene(scene), m_item(item)
    {
        setText("Add Item");
    }

    void undo() override
    {
        m_scene->removeItem(m_item);
    }

    void redo() override
    {
        //m_scene->addItem(m_item);
        if (!m_added)
           {
               // Scene zaten ekledi → sadece referans al
               m_added = true;
           }
           else
           {
               m_scene->addItem(m_item);
           }
    }

private:
    DiagramScene *m_scene;
    DiagramItem *m_item;
    bool m_added = false;
};
#endif // ADDITEMCOMMAND_H

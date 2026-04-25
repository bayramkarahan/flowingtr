#ifndef DELETECOMMAND_H
#define DELETECOMMAND_H
class DeleteCommand : public QUndoCommand
{
public:
    DeleteCommand(DiagramScene *scene, QGraphicsItem *item)
        : m_scene(scene)
    {
        m_items.append(item);
        setText("Delete Item");
    }

    DeleteCommand(DiagramScene *scene, QList<QGraphicsItem*> items)
        : m_scene(scene), m_items(items)
    {
        setText("Delete Items");
    }

    void undo() override
    {
        for (auto *item : m_items)
            m_scene->addItem(item);
    }

    void redo() override
    {
        for (auto *item : m_items)
            m_scene->removeItem(item);
    }

private:
    DiagramScene *m_scene;
    QList<QGraphicsItem*> m_items;
};
#endif // DELETECOMMAND_H

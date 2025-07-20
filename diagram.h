#ifndef DIAGRAM_H
#define DIAGRAM_H
#include <QGraphicsPixmapItem>
#include <QList>
#include<QtMath>
#include<QPainter>

QT_BEGIN_NAMESPACE
class QPixmap;
class QGraphicsItem;
class QGraphicsScene;
class QTextEdit;
class QGraphicsSceneMouseEvent;
class QMenu;
class QGraphicsSceneContextMenuEvent;
class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;
class QPolygonF;
QT_END_NAMESPACE

class Arrow;

//! [0]
class Diagram
{
public:
     enum DiagramType {NoType,Cizgi,Ok,CiftOk,Ucgen,Dortgen,Cember,
                       Baklava,ParalelKenar,Yamuk,Besgen,
                       Altigen,Sekizgen,DikUcgen,
                       GuzelYazi,Muzik,CizgiliSayfa,DrawPoligon,
                       Link,Loop,Output, Conditional, StartEnd, Input,Start,Process,End,Play };

       /* DiagramItem(DiagramType diagramType, QMenu *contextMenu, QGraphicsItem *parent = 0);

    void removeArrow(Arrow *arrow);
    void removeArrows();
    DiagramType diagramType() const { return myDiagramType; }
    QPolygonF polygon() const { return myPolygon; }
    void addArrow(Arrow *arrow);
    QPixmap image() const;
    int type() const override { return Type;}
*/
     //! [4]
     QPixmap image(Diagram::DiagramType type, int w, int h)
     {
         QPixmap pixmap(w, h);
         pixmap.fill(Qt::transparent);

         QPainter painter(&pixmap);
         painter.setRenderHint(QPainter::Antialiasing); // Daha düzgün kenarlar
         painter.setPen(QPen(Qt::black, 3));
         if(type==DiagramType::Link) painter.setPen(QPen(Qt::black, 1.5));
         QRectF rect(w*0.1, h * 0.05, w*0.8, h * 0.6); // Y 0.2h’den başlıyor
         painter.drawPolyline(sekilStore(type, rect));
         return pixmap;
     }

     //! [4]

     QPolygonF  sekilStore(Diagram::DiagramType sek, QRectF rect)
     {


         QPainterPath path;
         int r=100,d=100;

         QRectF lb(0, 0, 2*r, 2*r), rb(d, 0, 2*r, 2*r);
         qreal a1(90), a2(270), a(180);

              //int en=40;
         //int boy=40;
         QPolygonF result;
         if(sek==Diagram::DiagramType::Start)
         {
             //path.addRoundedRect(QRectF(-75, -50, 150, 100), 75, 75);
             path.addRoundedRect(rect, rect.width()/2, rect.height()/2);

            result = path.toFillPolygon();
         }
         if(sek==Diagram::DiagramType::End)
         {
           //  path.addRoundedRect(QRectF(-75, -50, 150, 100), 75, 75);
             path.addRoundedRect(rect, rect.width()/2, rect.height()/2);

            result = path.toFillPolygon();
         }
         if(sek==Diagram::DiagramType::Link)
         {
             //path.addRoundedRect(QRectF(-30, -30, 60 ,60), 75, 75);
             path.addRoundedRect(rect, rect.width(), rect.height());
            result = path.toFillPolygon();


         }
         if(sek==Diagram::DiagramType::Conditional)
         {
            /* result << QPointF(-100, 0) << QPointF(0, 50)
                       << QPointF(100, 0) << QPointF(0, -50)
                       << QPointF(-100, 0);*/
             result
                 << QPointF(rect.left(), rect.center().y())                  // (-100, 0)
                 << QPointF(rect.center().x(), rect.bottom())                // (0, 50)
                 << QPointF(rect.right(), rect.center().y())                 // (100, 0)
                 << QPointF(rect.center().x(), rect.top())                   // (0, -50)
                 << QPointF(rect.left(), rect.center().y());                 // (-100, 0)
         }
         if(sek==Diagram::DiagramType::Process)
         {
             /*result << QPointF(-100, -50) << QPointF(100, -50)
                       << QPointF(100, 50) << QPointF(-100, 50)
                       << QPointF(-100, -50);*/

              result << QPointF(rect.left(), rect.top())
                 << QPointF(rect.right(), rect.top())
                 << QPointF(rect.right(), rect.bottom())
                 << QPointF(rect.left(), rect.bottom())
                 << QPointF(rect.left(), rect.top());

       }
     if(sek==Diagram::DiagramType::Input)
     {
          /* result << QPointF(-100, 50) << QPointF(-70, -50)
                   << QPointF(100, -50) << QPointF(70, 50)
                   << QPointF(-100,50);*/

          result  << QPointF(rect.left(), rect.bottom())                        // (-100, 50)
              << QPointF(rect.left() + rect.width() * 0.2, rect.top())      // (-70, -50)
              << QPointF(rect.right(), rect.top())                          // (100, -50)
              << QPointF(rect.right() - rect.width() * 0.2, rect.bottom())  // (70, 50)
              << QPointF(rect.left(), rect.bottom());                       // (-100, 50)


     }
     if(sek==Diagram::DiagramType::Loop)
     {
       /*    result << QPointF(-80, 50) << QPointF(-120, 0)
                   << QPointF(-80, -50) << QPointF(80, -50)
                   << QPointF(120,0) << QPointF(80, 50)
                       << QPointF(-80, 50);*/
         result
             << QPointF(rect.left() + rect.width() * 0.2, rect.bottom())         // (-80, 50)
             << QPointF(rect.left(), rect.center().y())                         // (-120, 0)
             << QPointF(rect.left() + rect.width() * 0.2, rect.top())           // (-80, -50)
             << QPointF(rect.right() - rect.width() * 0.2, rect.top())          // (80, -50)
             << QPointF(rect.right(), rect.center().y())                        // (120, 0)
             << QPointF(rect.right() - rect.width() * 0.2, rect.bottom())       // (80, 50)
             << QPointF(rect.left() + rect.width() * 0.2, rect.bottom());       // (-80, 50)

     }
     if(sek==Diagram::DiagramType::Output)
     {
      /*   path.moveTo(-100, -50);
         path.lineTo(100, -50);
         path.lineTo(100, 0);
         path.cubicTo(100, 0, 40, 50, 40, 50);
         path.cubicTo(30,80, -80, 90, -100, 50);
         path.lineTo(-100, -40);
         QTransform transform;
         transform.scale(1.0, 1.0);  // Sadece Y yönünde küçültme
         QPainterPath scaledPath = transform.map(path);
         result = scaledPath.toFillPolygon();*/
         QRectF r = rect; // Kolaylık için
         path.moveTo(r.left(), r.top());                              // Sol üst
         path.lineTo(r.right(), r.top());                             // Sağ üst
         path.lineTo(r.right(), r.center().y());                      // Sağ ortadan aşağıya

         // İlk kıvrım (sağdan sola)
         path.cubicTo(r.right(), r.center().y(),
                      r.left() + r.width() * 0.6, r.bottom(),
                      r.left() + r.width() * 0.6, r.bottom());

         // İkinci kıvrım (alt ortadan sola)
         path.cubicTo(r.left() + r.width() * 0.5, r.bottom() + r.height() * 0.3,
                      r.left() + r.width() * 0.2, r.bottom() + r.height() * 0.4,
                      r.left(), r.bottom());

         // Sol ortadan yukarıya
         path.lineTo(r.left(), r.top() + r.height() * 0.1);

         result = path.toFillPolygon();


     }
     if(sek==Diagram::DiagramType::Play)
     {
         result << QPointF(-80, -80) << QPointF(-80, 80)
                   << QPointF(80, 0)  << QPointF(-80, -80);


     }

         if(sek==Diagram::DiagramType::CizgiliSayfa)
                    {
                  QPolygon result(65);     //GuzelYazi
                  result.setPoint(0, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(1, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(2, rect.right(), rect.bottom()-rect.height()/15);
                  result.setPoint(3, rect.left(), rect.bottom()-rect.height()/15);
                  result.setPoint(4, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(5, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(6, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(7, rect.right(), rect.top()+rect.height()/15*2);
                  result.setPoint(8, rect.left(), rect.top()+rect.height()/15*2);
                  result.setPoint(9, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(10, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(11, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(12, rect.right(), rect.top()+rect.height()/15*3);
                  result.setPoint(13, rect.left(), rect.top()+rect.height()/15*3);
                  result.setPoint(14, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(15, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(16, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(17, rect.right(), rect.top()+rect.height()/15*4);
                  result.setPoint(18, rect.left(), rect.top()+rect.height()/15*4);
                  result.setPoint(19, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(20, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(21, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(22, rect.right(), rect.top()+rect.height()/15*5);
                  result.setPoint(23, rect.left(), rect.top()+rect.height()/15*5);
                  result.setPoint(24, rect.left(), rect.top()+rect.height()/15);

         /***************************************************************************/
                  result.setPoint(25, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(26, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(27, rect.right(), rect.top()+rect.height()/15*6);
                  result.setPoint(28, rect.left(), rect.top()+rect.height()/15*6);
                  result.setPoint(29, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(30, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(31, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(32, rect.right(), rect.top()+rect.height()/15*7);
                  result.setPoint(33, rect.left(), rect.top()+rect.height()/15*7);
                  result.setPoint(34, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(35, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(36, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(37, rect.right(), rect.top()+rect.height()/15*8);
                  result.setPoint(38, rect.left(), rect.top()+rect.height()/15*8);
                  result.setPoint(39, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(40, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(41, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(42, rect.right(), rect.top()+rect.height()/15*9);
                  result.setPoint(43, rect.left(), rect.top()+rect.height()/15*9);
                  result.setPoint(44, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(45, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(46, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(47, rect.right(), rect.top()+rect.height()/15*10);
                  result.setPoint(48, rect.left(), rect.top()+rect.height()/15*10);
                  result.setPoint(49, rect.left(), rect.top()+rect.height()/15);
                  /***********************************************************************/
                  result.setPoint(50, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(51, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(52, rect.right(), rect.top()+rect.height()/15*11);
                  result.setPoint(53, rect.left(), rect.top()+rect.height()/15*11);
                  result.setPoint(54, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(55, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(56, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(57, rect.right(), rect.top()+rect.height()/15*12);
                  result.setPoint(58, rect.left(), rect.top()+rect.height()/15*12);
                  result.setPoint(59, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(60, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(61, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(62, rect.right(), rect.top()+rect.height()/15*13);
                  result.setPoint(63, rect.left(), rect.top()+rect.height()/15*13);
                  result.setPoint(64, rect.left(), rect.top()+rect.height()/15);
                      return result;
                  }

         if(sek==Diagram::DiagramType::GuzelYazi)
                    {
                  QPolygon result(55);     //GuzelYazi
                  result.setPoint(0, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(1, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(2, rect.right(), rect.bottom()-rect.height()/15);
                  result.setPoint(3, rect.left(), rect.bottom()-rect.height()/15);
                  result.setPoint(4, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(5, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(6, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(7, rect.right(), rect.top()+rect.height()/15*2);
                  result.setPoint(8, rect.left(), rect.top()+rect.height()/15*2);
                  result.setPoint(9, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(10, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(11, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(12, rect.right(), rect.top()+rect.height()/15*3);
                  result.setPoint(13, rect.left(), rect.top()+rect.height()/15*3);
                  result.setPoint(14, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(15, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(16, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(17, rect.right(), rect.top()+rect.height()/15*4);
                  result.setPoint(18, rect.left(), rect.top()+rect.height()/15*4);
                  result.setPoint(19, rect.left(), rect.top()+rect.height()/15);
         /***************************************************************************/
                  result.setPoint(20, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(21, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(22, rect.right(), rect.top()+rect.height()/15*6);
                  result.setPoint(23, rect.left(), rect.top()+rect.height()/15*6);
                  result.setPoint(24, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(25, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(26, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(27, rect.right(), rect.top()+rect.height()/15*7);
                  result.setPoint(28, rect.left(), rect.top()+rect.height()/15*7);
                  result.setPoint(29, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(30, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(31, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(32, rect.right(), rect.top()+rect.height()/15*8);
                  result.setPoint(33, rect.left(), rect.top()+rect.height()/15*8);
                  result.setPoint(34, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(35, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(36, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(37, rect.right(), rect.top()+rect.height()/15*9);
                  result.setPoint(38, rect.left(), rect.top()+rect.height()/15*9);
                  result.setPoint(39, rect.left(), rect.top()+rect.height()/15);
                  /***********************************************************************/
                  result.setPoint(40, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(41, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(42, rect.right(), rect.top()+rect.height()/15*11);
                  result.setPoint(43, rect.left(), rect.top()+rect.height()/15*11);
                  result.setPoint(44, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(45, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(46, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(47, rect.right(), rect.top()+rect.height()/15*12);
                  result.setPoint(48, rect.left(), rect.top()+rect.height()/15*12);
                  result.setPoint(49, rect.left(), rect.top()+rect.height()/15);

                  result.setPoint(50, rect.left(), rect.top()+rect.height()/15);
                  result.setPoint(51, rect.right(), rect.top()+rect.height()/15);
                  result.setPoint(52, rect.right(), rect.top()+rect.height()/15*13);
                  result.setPoint(53, rect.left(), rect.top()+rect.height()/15*13);
                  result.setPoint(54, rect.left(), rect.top()+rect.height()/15);
                      return result;
                  }
         if(sek==Diagram::DiagramType::Muzik)
                    {
                  QPolygon result(70);     //Muzik Desen
                  result.setPoint(0, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(1, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(2, rect.right(), rect.bottom()-rect.height()/18);
                  result.setPoint(3, rect.left(), rect.bottom()-rect.height()/18);
                  result.setPoint(4, rect.left(), rect.top()+rect.height()/18);

                  result.setPoint(5, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(6, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(7, rect.right(), rect.top()+rect.height()/18*2);
                  result.setPoint(8, rect.left(), rect.top()+rect.height()/18*2);
                  result.setPoint(9, rect.left(), rect.top()+rect.height()/18);

                  result.setPoint(10, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(11, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(12, rect.right(), rect.top()+rect.height()/18*3);
                  result.setPoint(13, rect.left(), rect.top()+rect.height()/18*3);
                  result.setPoint(14, rect.left(), rect.top()+rect.height()/18);

                  result.setPoint(15, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(16, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(17, rect.right(), rect.top()+rect.height()/18*4);
                  result.setPoint(18, rect.left(), rect.top()+rect.height()/18*4);
                  result.setPoint(19, rect.left(), rect.top()+rect.height()/18);

                  result.setPoint(20, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(21, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(22, rect.right(), rect.top()+rect.height()/18*5);
                  result.setPoint(23, rect.left(), rect.top()+rect.height()/18*5);
                  result.setPoint(24, rect.left(), rect.top()+rect.height()/18);
         /***************************************************************************/
                  result.setPoint(25, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(26, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(27, rect.right(), rect.top()+rect.height()/18*7);
                  result.setPoint(28, rect.left(), rect.top()+rect.height()/18*7);
                  result.setPoint(29, rect.left(), rect.top()+rect.height()/18);

                  result.setPoint(30, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(31, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(32, rect.right(), rect.top()+rect.height()/18*8);
                  result.setPoint(33, rect.left(), rect.top()+rect.height()/18*8);
                  result.setPoint(34, rect.left(), rect.top()+rect.height()/18);

                  result.setPoint(35, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(36, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(37, rect.right(), rect.top()+rect.height()/18*9);
                  result.setPoint(38, rect.left(), rect.top()+rect.height()/18*9);
                  result.setPoint(39, rect.left(), rect.top()+rect.height()/18);

                  result.setPoint(40, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(41, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(42, rect.right(), rect.top()+rect.height()/18*10);
                  result.setPoint(43, rect.left(), rect.top()+rect.height()/18*10);
                  result.setPoint(44, rect.left(), rect.top()+rect.height()/18);

                  result.setPoint(45, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(46, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(47, rect.right(), rect.top()+rect.height()/18*11);
                  result.setPoint(48, rect.left(), rect.top()+rect.height()/18*11);
                  result.setPoint(49, rect.left(), rect.top()+rect.height()/18);
                  /***********************************************************************/
                  result.setPoint(50, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(51, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(52, rect.right(), rect.top()+rect.height()/18*13);
                  result.setPoint(53, rect.left(), rect.top()+rect.height()/18*13);
                  result.setPoint(54, rect.left(), rect.top()+rect.height()/18);

                  result.setPoint(55, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(56, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(57, rect.right(), rect.top()+rect.height()/18*14);
                  result.setPoint(58, rect.left(), rect.top()+rect.height()/18*14);
                  result.setPoint(59, rect.left(), rect.top()+rect.height()/18);

                  result.setPoint(60, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(61, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(62, rect.right(), rect.top()+rect.height()/18*15);
                  result.setPoint(63, rect.left(), rect.top()+rect.height()/18*15);
                  result.setPoint(64, rect.left(), rect.top()+rect.height()/18);

                  result.setPoint(65, rect.left(), rect.top()+rect.height()/18);
                  result.setPoint(66, rect.right(), rect.top()+rect.height()/18);
                  result.setPoint(67, rect.right(), rect.top()+rect.height()/18*16);
                  result.setPoint(68, rect.left(), rect.top()+rect.height()/18*16);
                  result.setPoint(69, rect.left(), rect.top()+rect.height()/18);

                      return result;
                  }
         if(sek==Diagram::DiagramType::Cizgi)
           {
         QPolygon result(2);     //Çizgi
         result.setPoint(0,rect.left(), rect.top());
         result.setPoint(1, rect.right(), rect.bottom());
         return result;
         }
       /*  if(sek==Diagram::DiagramType::Kalem)
           {
         QPolygon result(2);     //Çizgi
         result.setPoint(0,rect.left(), rect.top()+rect.height()/2);
         result.setPoint(1, rect.right(), rect.bottom()-rect.height()/2);
         return result;
         }
*/
         if(sek==Diagram::DiagramType::Ok)
           {
         QPolygon result(7);     //Ok
         int r=20;
          QLineF line(QPoint(rect.left(),rect.top()), QPoint(rect.right(),rect.bottom()));

                    //double angle = std::atan2(-line.dy(), line.dx());
                    double angle = ::acos(line.dx() / line.length());
                        if (line.dy() >= 0)
                            angle = (3.14 * 2) - angle;

                                 QPointF arrowP1 =line.p2()-  QPointF(sin(angle + M_PI / 3) * r,
                                                                       cos(angle + M_PI / 3) * r);
                                 QPointF arrowP2 = line.p2()- QPointF(sin(angle + M_PI - M_PI / 3) * r,
                                                                       cos(angle + M_PI - M_PI / 3) * r);

         result.setPoint(0,rect.left(), rect.top());
         result.setPoint(1, rect.right(), rect.bottom());
         result.setPoint(2,arrowP1.x(),arrowP1.y());
         result.setPoint(3, rect.right(), rect.bottom());
         result.setPoint(4,arrowP2.x(),arrowP2.y());
         result.setPoint(5, rect.right(), rect.bottom());
         result.setPoint(6,rect.left(), rect.top());


         return result;
         }
         if(sek==Diagram::DiagramType::CiftOk)
           {
         QPolygon result(11);     //ÇiftOk
         int r=20;
          QLineF line(QPoint(rect.left(),rect.top()), QPoint(rect.right(),rect.bottom()));

                    //double angle = std::atan2(-line.dy(), line.dx());
                    double angle = ::acos(line.dx() / line.length());
                        if (line.dy() >= 0)
                            angle = (3.14 * 2) - angle;

                                 QPointF arrowP21 =line.p2()-  QPointF(sin(angle + M_PI / 3) * r,
                                                                       cos(angle + M_PI / 3) * r);
                                 QPointF arrowP22 = line.p2()- QPointF(sin(angle + M_PI - M_PI / 3) * r,
                                                                       cos(angle + M_PI - M_PI / 3) * r);
                                 QPointF arrowP11 =line.p1()+  QPointF(sin(angle + M_PI / 3) * r,
                                                                       cos(angle + M_PI / 3) * r);
                                 QPointF arrowP12 = line.p1()+ QPointF(sin(angle + M_PI - M_PI / 3) * r,
                                                                       cos(angle + M_PI - M_PI / 3) * r);

         result.setPoint(0,rect.left(), rect.top());
          result.setPoint(1,arrowP11.x(),arrowP11.y());
          result.setPoint(2,rect.left(), rect.top());
           result.setPoint(3,arrowP12.x(),arrowP12.y());
          result.setPoint(4,rect.left(), rect.top());

         result.setPoint(5, rect.right(), rect.bottom());
         result.setPoint(6,arrowP21.x(),arrowP21.y());
         result.setPoint(7, rect.right(), rect.bottom());
         result.setPoint(8,arrowP22.x(),arrowP22.y());
         result.setPoint(9, rect.right(), rect.bottom());
         result.setPoint(10,rect.left(), rect.top());

         return result;
         }
     if(sek==Diagram::DiagramType::Ucgen)
           {
           QPolygon result(4);     //Üçgen
           //rect.moveTo(-150, -150);
           result.setPoint(0, rect.center().x(), rect.top());
           result.setPoint(1, rect.right(), rect.bottom());
           result.setPoint(2, rect.left(), rect.bottom());
           result.setPoint(3, rect.center().x(), rect.top());
          // sekilLayout->addWidget(createCellWidget(tr("Üçgen"),result),1,0);
           return result;
           }
     if(sek==Diagram::DiagramType::Dortgen)
     {//Dörtgen
           QPolygon result(5);
          // rect.moveTo(-150, -150);
           result.setPoint(0, rect.left(), rect.top());
           result.setPoint(1, rect.right(), rect.top());
           result.setPoint(2, rect.right(), rect.bottom());
           result.setPoint(3, rect.left(), rect.bottom());
           result.setPoint(4, rect.left(), rect.top());
          // sekilLayout->addWidget(createCellWidget(tr("Dörtgen"),result),2,0);
           return result;
           }
     if(sek==Diagram::DiagramType::Yamuk)
     {  //Yamuk
           QPolygon result(5);
           //rect.moveTo(-150, -150);
           result.setPoint(0, rect.left(), rect.top());
           result.setPoint(1, rect.right()/2, rect.top());
           result.setPoint(2, rect.right(), rect.bottom());
           result.setPoint(3, rect.left(), rect.bottom());
           result.setPoint(4, rect.left(), rect.top());
         //  sekilLayout->addWidget(createCellWidget(tr("Yamuk"),result),3,0);
           return result;
           }

     if(sek==Diagram::DiagramType::ParalelKenar)
           {  //paralel kenar
           QPolygon result(5);
           //rect.moveTo(-150, -150);
           result.setPoint(0, rect.center().x()/2, rect.top());
           result.setPoint(1, rect.right(), rect.top());
           result.setPoint(2, rect.center().x()+rect.center().x()/2, rect.bottom());
           result.setPoint(3, rect.left(), rect.bottom());
           result.setPoint(4, rect.center().x()/2, rect.top());
        //   sekilLayout->addWidget(createCellWidget(tr("P.Kenar"),result),4,0);
           return result;
           }
     if(sek==Diagram::DiagramType::Baklava)
     {  //Dik paralel kenar
               QPolygon result(5);
               //rect.moveTo(-150, -150);
               result.setPoint(0, rect.center().x(), rect.top());
               result.setPoint(1, rect.right(), rect.top()+rect.height()/2);
               result.setPoint(2, rect.center().x(), rect.bottom());
               result.setPoint(3, rect.left(), rect.top()+rect.height()/2);
                result.setPoint(4, rect.center().x(), rect.top());
        //            sekilLayout->addWidget(createCellWidget(tr("D.Kenar"),result),5,0);
                return result;
               }
     if(sek==Diagram::DiagramType::Cember)
     {
         QPolygon result(2);     //Çizgi
         result.setPoint(0,rect.left(), rect.top());
         result.setPoint(1, rect.right(), rect.bottom());
         return result;
     }
     if(sek==Diagram::DiagramType::Besgen)
     {
         QPolygon result(6);     //Çizgi
         result.setPoint(0,rect.left(), rect.top()+rect.height()/2);
         result.setPoint(1, rect.center().x(), rect.top());
         result.setPoint(2,rect.right(), rect.top()+rect.height()/2);
         result.setPoint(3,rect.left()+rect.width()/4*3, rect.bottom());
         result.setPoint(4,rect.left()+rect.width()/4, rect.bottom());
         result.setPoint(5,rect.left(), rect.top()+rect.height()/2);
            return result;
     }
     if(sek==Diagram::DiagramType::Altigen)
     {
         QPolygon result(7);     //Çizgi
         result.setPoint(0,rect.left(), rect.top()+rect.height()/2);
         result.setPoint(1, rect.center().x()-rect.width()/4, rect.top());
         result.setPoint(2, rect.center().x()+rect.width()/4, rect.top());
         result.setPoint(3,rect.right(), rect.top()+rect.height()/2);
         result.setPoint(4, rect.center().x()+rect.width()/4, rect.bottom());
         result.setPoint(5, rect.center().x()-rect.width()/4, rect.bottom());
         result.setPoint(6,rect.left(), rect.top()+rect.height()/2);
         return result;
     }
     if(sek==Diagram::DiagramType::Sekizgen)
     {
         QPolygon result(9);     //Çizgi
         result.setPoint(0,rect.left(), rect.top()+rect.height()/4);
         result.setPoint(1, rect.center().x()-rect.width()/4, rect.top());
         result.setPoint(2, rect.center().x()+rect.width()/4, rect.top());
         result.setPoint(3,rect.right(), rect.top()+rect.height()/4);
         result.setPoint(4,rect.right(), rect.bottom()-rect.height()/4);
         result.setPoint(5, rect.center().x()+rect.width()/4, rect.bottom());
         result.setPoint(6, rect.center().x()-rect.width()/4, rect.bottom());
         result.setPoint(7,rect.left(), rect.bottom()-rect.height()/4);
         result.setPoint(8,rect.left(), rect.top()+rect.height()/4);
         return result;
     }
     if(sek==Diagram::DiagramType::DikUcgen)
     {
         QPolygon result(4);     //Çizgi
         result.setPoint(0, rect.left(), rect.top());
         result.setPoint(1, rect.right(), rect.bottom());
         result.setPoint(2, rect.left(), rect.bottom());
         result.setPoint(3, rect.left(), rect.top());
         return result;
     }


         return result;
     }

protected:
  //  void contextMenuEvent(QGraphicsSceneContextMenuEvent *event) override;
   // QVariant itemChange(GraphicsItemChange change, const QVariant &value) override;

private:
  //  DiagramType myDiagramType;
   // QPolygonF myPolygon;
   // QMenu *myContextMenu;
   // QList<Arrow *> arrows;
};
//! [0]
#endif // DIAGRAM_H

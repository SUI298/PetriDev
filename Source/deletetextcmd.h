#ifndef DELETETEXTCMD_H
#define DELETETEXTCMD_H

#include <QUndoCommand>
#include <QTextCursor>

/*!
 * \brief DeleteTextCmd Klasse ist der rueckgaengigmachbare Befehl zum loeschen eines Zeichns vor oder nach dem Textcursor.
 * \inherits QUndoCommand
 * \author Samuel Pitt
 */
class DeleteTextCmd : public QUndoCommand
{
public:
    /*!
     * \brief DeleteTextCmd Konstruktor.
     * \param cursor Textcursor
     * \param back nach vorne oder hinten loeschen? (irelevant wenn Text ausgewaehlt)
     */
    DeleteTextCmd(const QTextCursor& cursor, bool back)
    : back(back), cursor(cursor)
    {
        if(!this->cursor.hasSelection()){ //kein ausgewaehler text?
            //Zeichen davor oder dahinter auswaehlen (je nach Taste)
            this->cursor.movePosition(back ? QTextCursor::Left : QTextCursor::Right , QTextCursor::KeepAnchor);
        }
        text = this->cursor.selectedText(); //text speichern
        setText("delete text");
    }

    /*!
     * \brief id zum zusammenfuegen gleichartiger Befehle ist 3.
     * \return 3
     */
    int id()const override {return 3;}

    /*!
     * \brief redo ausfuehren des Befehls.
     */
    void redo() override
    {
        cursor.removeSelectedText(); //entferne Text
    }

    /*!
     * \brief undo rueckgaengig machen des Befehls.
     */
    void undo() override
    {
        cursor.insertText(text); //text einfuegen
        //text wieder auswahlen mit cursor
        cursor.movePosition(QTextCursor::Left, QTextCursor::MoveAnchor, text.length());
        cursor.movePosition(QTextCursor::Right, QTextCursor::KeepAnchor, text.length());
    }

    /*!
     * \brief mergeWith zusammenfuegen gleichartiger Befehle.
     * \param command Zeiger zu anderem Befehl
     * \return true bei erforlreichem zusmmenfuegen.
     */
    bool mergeWith(const QUndoCommand *command)override
    {
        if(id() == command->id()){ //gleicher Typ?
            const DeleteTextCmd* other = static_cast<const DeleteTextCmd*>(command);
            if( cursor == other->cursor && cursor.document() == other->cursor.document()){
                //Cursor und dessen Dokumente sind identisch
                //je nach loeschrichtung reienfolge beachten
                text = back ? other->text + text : text + other->text;
                return true; //Befehle zusammengefuegt.
            }
        }
        return false; //nicht zusammengefuegt.
    }

    bool back;
    QString text;
    QTextCursor cursor;

};

#endif // DELETETEXTCMD_H

#ifndef APPENDTEXTCMD_H
#define APPENDTEXTCMD_H

#include <QUndoCommand>
#include <QTextCursor>
#include <QDebug>

/*!
 * \brief AppendTextCmd Klasse ist der rueckgangigmachbare Befehl zum einfuegen von Text bei einem Textcursor.
 * \inherits QUndoCommand
 * \author Samuel Pitt
 */
class AppendTextCmd : public QUndoCommand
{
public:
    /*!
     * \brief AppendTextCmd Konstruktor.
     * \param cursor Textcursor
     * \param text Text
     */
    AppendTextCmd(const QTextCursor& cursor, const QString& text)
        :  text(text), cursorSelection(cursor.selectedText()), cursor(cursor)
    {
        setText("append text");
    }

    /*!
     * \brief id die Identitaet dieses Befehls ist 2
     * \return 2
     * \see mergeWith
     */
    int id()const override {return 2;}

    /*!
     * \brief redo ausfuehren des Befehls.
     */
    void redo() override
    {
        cursor.insertText(text);
    }

    /*!
     * \brief undo rueckgaengig machen des Befehls.
     */
    void undo() override
    {
        for(int i = 0; i < text.length(); i++){
            cursor.deletePreviousChar();
        }
        cursor.insertText(cursorSelection);
    }

    /*!
     * \brief mergeWith fuegt diesen Befehl mit gleichartigen zusammenin einen einzigen Befehl.
     * \param command Zeiger auf anderen Befehl
     * \return true falls erfolgreich.
     */
    bool mergeWith(const QUndoCommand *command)override
    {
        if(id() == command->id()){
            const AppendTextCmd* other = static_cast<const AppendTextCmd*>(command);
            if( cursor == other->cursor && cursor.document() == other->cursor.document()){ //cursor und dessen dokumente sind identisch                text += other->text; //text anfuegen
                return true; //zusammengefuegt
            }
        }
        return false; //nicht zusammengefuegt
    }

    QString text;
    QString cursorSelection;
    QTextCursor cursor;


};

#endif // APPENDTEXTCMD_H

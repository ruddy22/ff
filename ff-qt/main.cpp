#include <QApplication>
#include <QDebug>
#include <QMainWindow>
#include <QToolBox>
#include <QVBoxLayout>

#include "ff/types.h"

enum class TaskMode {
    Overdue,  //< end in past
    EndToday, //< end today
    EndSoon,  //< started, end in future
    Actual,   //< started, no end
    Starting, //< starting in future
};

const QList<TaskMode> taskModes {
    TaskMode::Overdue,
    TaskMode::EndToday,
    TaskMode::EndSoon,
    TaskMode::Actual,
    TaskMode::Starting,
};

const QMap<TaskMode, QString> sectionLabels {
    {TaskMode::Overdue,  "Overdue"},
    {TaskMode::EndToday, "Due today"},
    {TaskMode::EndSoon,  "Due soon"},
    {TaskMode::Actual,   "Actual"},
    {TaskMode::Starting, "Starting soon"},
};

class Agenda: public QToolBox {
public:
    Agenda(Storage * storage) {
        // TODO set the first item as current
        for (auto mode : taskModes)
            addSection(mode);
        auto notes = loadActiveNotes(storage);
        qDebug() << notes;
        // TODO:
        // for (auto note : notes)
        //     addNote(note);
    }
private:
    QMap<TaskMode, QVBoxLayout *> sections;
    void addSection(TaskMode mode) {
        auto widget = new QWidget;
        addItem(widget, sectionLabel(mode, 0));
        auto box = new QVBoxLayout(widget);
        box->addStretch();
        sections[mode] = box;
    }
    static const QString sectionLabel(TaskMode mode, int n) {
        return QString("%1 (%2)").arg(sectionLabels[mode]).arg(n);
    }
    void addNote(NoteView * /*note*/) {
        // TODO:
        // today <- getUtcToday
        // item  <- newNoteWidget note
        // let mode       = taskMode today note
        // let sectionBox = FullMap.lookup mode modeSections
        // n <- sectionSize sectionBox
        // insertWidget sectionBox n item
        // setItemText(int(mode), sectionLabel(mode, n + 1));
    }
};

class MainWindow: public QMainWindow {
public:
    MainWindow(Storage * storage)
        : storage(storage)
    {
        setWindowTitle("ff");
        setCentralWidget(makeTabs());
    }
private:
    Storage * storage;
    QTabWidget * makeTabs() {
        auto tabs = new QTabWidget(this);
        tabs->addTab(new Agenda(storage), "Agenda");
        return tabs;
    }
};

int main(int argc, char * argv[]) {
    // TODO Config { dataDir = Just dataDir } <- loadConfig
    // TODO timeVar                           <- newTVarIO =<< getRealLocalTime
    Storage * storage = nullptr; // TODO dataDir timeVar

    QApplication app(argc, argv);
    app.setOrganizationDomain("ff.cblp.su");
    app.setOrganizationName("ff");
    app.setApplicationName("ff");
    // TODO setApplicationVersion app $ showVersion version

    auto mainWindow = new MainWindow(storage);
    mainWindow->show();
    app.exec();
    return 0;
}

// TODO
//     -- https://wiki.qt.io/Saving_Window_Size_State
//     void $ onEvent this $ \(_ :: QShowEvent) -> do
//         withScopedPtr QSettings.new $ \settings -> do
//             void
//                 $   value settings "mainWindowGeometry"
//                 >>= toByteArray
//                 >>= restoreGeometry this
//             void
//                 $   value settings "mainWindowState"
//                 >>= toByteArray
//                 >>= restoreState this
//         pure False
//     void $ onEvent this $ \(_ :: QCloseEvent) -> do
//         withScopedPtr QSettings.new $ \settings -> do
//             setValue settings "mainWindowGeometry"
//                 =<< QVariant.newWithByteArray
//                 =<< saveGeometry this
//             setValue settings "mainWindowState"
//                 =<< QVariant.newWithByteArray
//                 =<< saveState this
//         pure False
//     pure this

// sectionIndices :: ModeMap Int
// sectionIndices = FullMap fromEnum

// newDateWidget :: String -> Day -> IO QHBoxLayout
// newDateWidget label date = do
//     box <- QHBoxLayout.new
//     addWidget box =<< QLabel.newWithText label
//     addWidget box =<< do
//         dateEdit <-
//             QDateEdit.newWithDate
//                 =<< QDate.newWithYearMonthDay (fromInteger y) m d
//         setCalendarPopup dateEdit True
//         setReadOnly      dateEdit True
//         pure dateEdit
//     pure box
//     where (y, m, d) = toGregorian date

// newNoteWidget :: NoteView -> IO QFrame
// newNoteWidget NoteView { text, start, end } = do
//     this <- QFrame.new
//     setFrameShape this StyledPanel
//     do
//         box <- QVBoxLayout.newWithParent this
//         addWidget box =<< QLabel.newWithText (Text.unpack text)
//         addLayout box =<< do
//             fieldsBox <- QHBoxLayout.new
//             addLayout fieldsBox =<< newDateWidget "Start:" start
//             whenJust end $ addLayout fieldsBox <=< newDateWidget "Deadline:"
//             addStretch fieldsBox
//             pure fieldsBox
//     pure this

// -- Because last item is always a stretch.
// sectionSize :: QLayoutConstPtr layout => layout -> IO Int
// sectionSize layout = pred <$> count layout

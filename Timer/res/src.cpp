QAction * actionAbout = ui->menuBar->addAction (qApp->translate("About", "About"));
connect (actionAbout, &QAction::triggered,
		 [=]() {
	QDialog dialog(this);
	dialog.setWindowTitle(actionAbout->text());
	dialog.setLayout (new QHBoxLayout());
	const QString text = qApp->translate("About", "Made by %1 for training club %2.").
						 arg("<a href='mailto:EliseyZamahov@gmail.com'>Elisey Zamahov</a>").
						 arg("Advance");
	dialog.layout()->addWidget (new QLabel(text, &dialog));
	dialog.exec ();
	});
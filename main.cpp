/****************************************************************************
  **
  ** Copyright (C) 2016 The Qt Company Ltd.
  ** Contact: https://www.qt.io/licensing/
  **
  ** This file is part of the Qt Charts module of the Qt Toolkit.
  **
  ** $QT_BEGIN_LICENSE:GPL$
  ** Commercial License Usage
  ** Licensees holding valid commercial Qt licenses may use this file in
  ** accordance with the commercial license agreement provided with the
  ** Software or, alternatively, in accordance with the terms contained in
  ** a written agreement between you and The Qt Company. For licensing terms
  ** and conditions see https://www.qt.io/terms-conditions. For further
  ** information use the contact form at https://www.qt.io/contact-us.
  **
  ** GNU General Public License Usage
  ** Alternatively, this file may be used under the terms of the GNU
  ** General Public License version 3 or (at your option) any later version
  ** approved by the KDE Free Qt Foundation. The licenses are as published by
  ** the Free Software Foundation and appearing in the file LICENSE.GPL3
  ** included in the packaging of this file. Please review the following
  ** information to ensure the GNU General Public License requirements will
  ** be met: https://www.gnu.org/licenses/gpl-3.0.html.
  **
  ** $QT_END_LICENSE$
  **
  ****************************************************************************/

  #include <QtWidgets/QApplication>
  #include <QtWidgets/QMainWindow>
  #include <QtCharts/QChartView>
  #include <QtCharts/QLineSeries>
  #include <QtCharts/QValueAxis>
  #include <QtCharts/QAreaSeries>
  #include <QtCore/QDateTime>
  #include <QtCharts/QDateTimeAxis>
  #include <QtCore/QFile>
  #include <QtCore/QTextStream>
  #include <QtCore/QDebug>
  #include <QtCharts/QValueAxis>

  QT_CHARTS_USE_NAMESPACE

  int main(int argc, char *argv[])
  {
	  QApplication a(argc, argv);
	  qsrand(QTime(0, 0, 0).secsTo(QTime::currentTime()));

	  QLineSeries *seriesA = new QLineSeries();
	  QLineSeries *seriesB = new QLineSeries();

	  // data from http://www.swpc.noaa.gov/ftpdir/weekly/RecentIndices.txt
	  // http://www.swpc.noaa.gov/ftpdir/weekly/README
	  // http://www.weather.gov/disclaimer
	  QFile sunSpots("sun_spots.txt");
	  if (!sunSpots.open(QIODevice::ReadOnly | QIODevice::Text)) {
		  return 1;
	  }

	  QTextStream stream(&sunSpots);
	  while (!stream.atEnd()) {
		  QString line = stream.readLine();
		  if (line.startsWith("#") || line.startsWith(":"))
			  continue;
		  QStringList values = line.split(" ", QString::SkipEmptyParts);
		  QDateTime momentInTime;
		  momentInTime.setDate(QDate(values[0].toInt(), values[1].toInt() , 15));
		  seriesA->append(momentInTime.toMSecsSinceEpoch(), values[2].toDouble());
		  seriesB->append(momentInTime.toMSecsSinceEpoch(), values[2].toDouble() * 2);
	  }
	  sunSpots.close();

	  QAreaSeries * series = new QAreaSeries(seriesA, seriesB);

	  QChart *chart = new QChart();
	  chart->addSeries(seriesA);
	  chart->addSeries(seriesB);
	  chart->addSeries(series);
	  chart->legend()->hide();
	  chart->setTitle("Sunspots count (by Space Weather Prediction Center)");

	  QDateTimeAxis *axisX = new QDateTimeAxis;
	  axisX->setTickCount(10);
	  axisX->setFormat("MMM yyyy");
	  axisX->setTitleText("Date");
	  chart->addAxis(axisX, Qt::AlignBottom);

	  QValueAxis *axisY = new QValueAxis;
	  axisY->setLabelFormat("%i");
	  axisY->setTitleText("Sunspots count");
	  chart->addAxis(axisY, Qt::AlignLeft);

	  seriesA->attachAxis(axisX);
	  seriesA->attachAxis(axisY);
	  seriesB->attachAxis(axisX);
	  seriesB->attachAxis(axisY);

	  // TODO: make generic for a list of QLineSeries'
	  {
		  qreal min = 0;
		  qreal max = 0;
		  for (int i = 0; i < seriesA->count(); ++i) {
			  if (min > seriesA->at(i).y()) min = seriesA->at(i).y();
			  if (max < seriesA->at(i).y()) max = seriesA->at(i).y();
		  }
		  for (int i = 0; i < seriesB->count(); ++i) {
			  if (min > seriesB->at(i).y()) min = seriesB->at(i).y();
			  if (max < seriesB->at(i).y()) max = seriesB->at(i).y();
		  }
		  axisY->setRange(min, max);
	  }

	  QChartView *chartView = new QChartView(chart);
	  chartView->setRenderHint(QPainter::Antialiasing);

	  QMainWindow window;
	  window.setCentralWidget(chartView);
	  window.resize(820, 600);
	  window.show();

	  return a.exec();
  }

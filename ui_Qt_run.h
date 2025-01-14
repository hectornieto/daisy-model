// ui_Qt_run.h -- Qt based top level user interface for running a program.
// 
// Copyright 2007 Per Abrahamsen and KVL.
//
// This file is part of Daisy.
// 
// Daisy is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser Public License as published by
// the Free Software Foundation; either version 2.1 of the License, or
// (at your option) any later version.
// 
// Daisy is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser Public License for more details.
// 
// You should have received a copy of the GNU Lesser Public License
// along with Daisy; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

#ifndef UI_QT_RUN_H
#define UI_QT_RUN_H

#include "ui_Qt.h"
#include "vis_Qt.h"
#include "toplevel.h"
#include "memutils.h"
#include <map>
#include <boost/scoped_ptr.hpp>

#include <QtCore/QPointer>

class QPushButton;
class QLabel;
class QComboBox;
class QVBoxLayout;

class Topelevel;
class Block;
class RunQtMain;
class UIFilter;
class FrameModel;
class UIItem;

class UIRun : public QWidget, public UIQt
{
  Q_OBJECT

  // Run widgets.
  VisQtMain qt_main;
  QPointer<QLabel> qt_name;
  QPointer<QLabel> qt_file;
  QPointer<QLabel> qt_description;               
  QPointer<QPushButton> qt_runstop;
  QPointer<VisQtProgress> qt_progress;

  // Edit Widgets
  QPointer<QVBoxLayout> qt_edit_pane;
  QPointer<QComboBox> qt_select_filter;
  QPointer<QComboBox> qt_select_component;
  QPointer<QComboBox> qt_select_model;
  QPointer<QComboBox> qt_new_component;
  QPointer<QComboBox> qt_new_parent;

  // Edit state.
  symbol selected_file;
  boost::scoped_ptr<UIFilter> filter;
  symbol selected_filter;
  symbol selected_component;
  symbol selected_model;
  symbol selected_new_component;
  symbol selected_new_parent;

  // Logs.
  bool has_loaded_log_file;
  auto_vector<Log*> all_logs;
  std::map<symbol, LogQt*> logs;
  void build_log (Metalib&, Block& block, const std::string& log);
  bool attach_log (const std::string& log, VisQtLog* object) const;

  // The simulation thread.
private:
  Toplevel* top_level;
  QPointer<RunQtMain> qt_run;
private slots:
  void run_program ();
  void stop_program ();

  // Use.
public:
  void attach (Toplevel& toplevel);
private:
  void attach_tab_file (Toplevel&, QTabWidget&);
  void attach_tab_edit (Toplevel&, QTabWidget&);
  void attach_tab_run (Toplevel&, QTabWidget&);
public:
  void run (Toplevel& toplevel);
  void failure (Toplevel& toplevel);

  // Update.
private:
  void reset ();
  void reset_tab_edit ();
  void reset_tab_run ();

  // File actions.
private slots:
  void new_setup ();
  void open_setup ();
  void save_setup ();
  void save_setup_as ();

  // Edit actions.
private slots:
  void select_filter (const QString&);
  void select_component (const QString&);
  void select_model (const QString&);
  void select_new_component (const QString&);
  void select_new_parent (const QString&);

private:
  void select_file (symbol);
  void select_filter (symbol);
  void select_component (symbol);
  void select_model (symbol);
  QPointer<QWidget>
  build_item (const Metalib& metalib, 
              const symbol component, const symbol model,
              const UIFilter& filter, const UIItem& item);
  void select_new_component (symbol);
  void select_new_parent (symbol);

  // Track simulation.
public slots:
  void new_state (Toplevel::state_t);

  // Create.
private:
  explicit UIRun ();            // Disable.
public:
  explicit UIRun (const BlockModel& al);
private:
  ~UIRun ();
};

#endif // UI_QT_RUN_H

// ui_Qt_run.h ends here.

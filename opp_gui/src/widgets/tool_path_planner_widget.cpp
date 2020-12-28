/*
 * Copyright 2019 Southwest Research Institute
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "opp_gui/widgets/tool_path_planner_widget.h"

#include <map>
#include <regex>
#include <string>

#include <QFileDialog>
#include <QMessageBox>
#include <QTableView>

#include <geometric_shapes/mesh_operations.h>
#include <geometric_shapes/shape_operations.h>
#include <ros/console.h>

#include "opp_gui/utils.h"
#include "opp_gui/widgets/tool_path_editor_widget.h"
#include "opp_gui/widgets/touch_point_editor_widget.h"
#include "ui_tool_path_planner.h"

const static std::string MESH_MARKER_TOPIC = "mesh_marker";
const static int MIN_TOUCH_POINTS = 0;
const static int MIN_VERIFICATION_POINTS = 0;

namespace opp_gui
{
ToolPathPlannerWidget::ToolPathPlannerWidget(QWidget* parent,
                                             const ros::NodeHandle& nh,
                                             const std::vector<std::string>& frames)
  : QWidget(parent)
  , nh_(nh)
{
  ui_ = new Ui::ToolPathPlanner();
  ui_->setupUi(this);

  // Add the available tf frame
  if (!frames.empty())
  {
    marker_frame_ = frames.front();
  }
  else
  {
    ROS_ERROR_STREAM("No visualization frames available.  Using \"map\" instead.");
    marker_frame_ = "map";
  }

  // Create the other widgets
  tool_path_parameters_editor_widget_ = new ToolPathParametersEditorWidget(nh_, this);
//  touch_point_editor_ = new TouchPointEditorWidget(this, nh_, marker_frame_);
//  verification_point_editor_ = new TouchPointEditorWidget(this, nh_, marker_frame_);
//  tool_path_editor_ = new ToolPathEditorWidget(this, nh_, marker_frame_);

//   Set the color of the touch point markers
//  touch_point_editor_->setMarkerColor(1.0, 0.0, 0.0);
//  verification_point_editor_->setMarkerColor(0.0, 0.0, 1.0);

  // Add the widgets to the appropriate frames
  {
    // Tool path parameters editor
    QVBoxLayout* layout = new QVBoxLayout();
    layout->addWidget(tool_path_parameters_editor_widget_);
    ui_->widget_placeholder_tool_path_parameters_editor->setLayout(layout);
  }
//  {
//    // Touch point editor
//    QVBoxLayout* layout = new QVBoxLayout();
//    layout->addWidget(touch_point_editor_);
//    ui_->frame_define_touch_off_points->setLayout(layout);
//  }
//  {
//    // Verification point editor
//    QVBoxLayout* layout = new QVBoxLayout();
//    layout->addWidget(verification_point_editor_);
//    ui_->frame_define_verification_points->setLayout(layout);
//  }
//  {
//    // Tool path editor
//    QVBoxLayout* layout = new QVBoxLayout();
//    layout->addWidget(tool_path_editor_);
//    ui_->frame_define_toolpaths->setLayout(layout);
//  }

//  // Disable the "downstream" tabs until a model is loaded
//  setModelTabsEnabled(false);
//  setJobTabsEnabled(false, false);

//  // Connect the signals and slots
//  connect(ui_->push_button_find_model_file, &QPushButton::clicked, this, &ToolPathPlannerWidget::browseForMeshResource);
//  connect(ui_->push_button_load_parts_from_database,
//          &QPushButton::clicked,
//          this,
//          &ToolPathPlannerWidget::loadModelsFromDatabase);
//  loadModelsFromDatabase();
//  connect(
//      ui_->list_widget_parts, &QListWidget::currentItemChanged, this, &ToolPathPlannerWidget::onModelSelectionChanged);
//  connect(ui_->push_button_load_selected_part, &QPushButton::clicked, this, &ToolPathPlannerWidget::loadSelectedModel);
//  connect(ui_->push_button_save_entry, &QPushButton::clicked, this, &ToolPathPlannerWidget::saveModel);

  // Add a publisher for the mesh marker
  pub_ = nh_.advertise<visualization_msgs::Marker>(MESH_MARKER_TOPIC, 1, true);
}

void ToolPathPlannerWidget::setVisualizationFrame(const QString& text)
{
  marker_frame_ = text.toStdString();
//  touch_point_editor_->setMarkerFrame(marker_frame_);
//  verification_point_editor_->setMarkerFrame(marker_frame_);
//  tool_path_editor_->setMarkerFrame(marker_frame_);
}

// Parts Page
//void ToolPathPlannerWidget::browseForMeshResource()
//{
//  QString filename = QFileDialog::getOpenFileName(this, "Load Model", "", "Mesh Files (*.stl *.ply *.obj)");
//  if (filename.isEmpty())
//  {
//    ROS_WARN_STREAM(__func__ << ": Empty filename");
//    return;
//  }

//  ui_->line_edit_model_filename->setText(filename);
//  loadMeshFromResource();
//  return;
//}

//void ToolPathPlannerWidget::loadMeshFromResource()
//{
//  // Get the filename and package of the model
//  std::string filename = ui_->line_edit_model_filename->text().toStdString();
//  if (filename.empty())
//  {
//    QMessageBox::warning(this, "Input Error", "Model filename or package name not specified");
//    return;
//  }

//  // Construct the mesh resource name using the package and filename
//  std::vector<std::string> file_extensions = { ".stl", ".ply", ".obj" };

//  mesh_resource_.clear();
//  for (const std::string& ext : file_extensions)
//  {
//    std::regex rgx(".*" + ext + "$");
//    std::smatch match;
//    if (std::regex_search(filename, match, rgx))
//    {
//      mesh_resource_ = "file://" + filename;
//      break;
//    }
//  }

//  if (mesh_resource_.empty())
//  {
//    std::string message = "Invalid mesh resource file extension. Acceptable inputs are: ";
//    for (const std::string& ext : file_extensions)
//      message += ext + " ";

//    QMessageBox::warning(this, "Input Error", QString(message.c_str()));
//    return;
//  }
//  ROS_INFO_STREAM("Attempting to load mesh from resource: '" << mesh_resource_ << "'");

//  if (!loadMesh())
//    return;
//}

//bool ToolPathPlannerWidget::loadMesh()
//{
//  // Attempt to load this file into a shape_msgs/Mesh type
//  shape_msgs::Mesh mesh;
//  if (!utils::getMeshMsgFromResource(mesh_resource_, mesh))
//  {
//    std::string message = "Failed to load mesh from resource: '" + mesh_resource_ + "'";
//    QMessageBox::warning(this, "Input Error", message.c_str());
//    return false;
//  }

//  // Initialize the tool path editor with the mesh
//  tool_path_editor_->init(mesh);

//  // Publish the mesh marker
//  visualization_msgs::Marker mesh_marker =
//      utils::createMeshMarker(0, "mesh", Eigen::Isometry3d::Identity(), marker_frame_, mesh_resource_);

//  pub_.publish(mesh_marker);

//  return true;
//}

}  // namespace opp_gui

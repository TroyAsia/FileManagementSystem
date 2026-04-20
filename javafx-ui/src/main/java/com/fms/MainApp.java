package com.fms;

import javafx.application.Application;
import javafx.geometry.Insets;
import javafx.scene.Scene;
import javafx.scene.control.Button;
import javafx.scene.control.Label;
import javafx.scene.control.TextArea;
import javafx.scene.control.TextField;
import javafx.scene.layout.GridPane;
import javafx.stage.Stage;

public class MainApp extends Application {
    private final FmsController controller = new FmsController();

    @Override
    public void start(Stage stage) {
        Label fileLabel = new Label("File name:");
        TextField fileField = new TextField();

        Button createBtn = new Button("Create");
        Button openBtn = new Button("Open");
        Button closeBtn = new Button("Close");
        Button searchBtn = new Button("Search");

        TextArea output = new TextArea();
        output.setEditable(false);
        output.setPrefRowCount(12);

        createBtn.setOnAction(e -> output.appendText(controller.runCommand("create", fileField.getText()) + "\n"));
        openBtn.setOnAction(e -> output.appendText(controller.runCommand("open", fileField.getText()) + "\n"));
        closeBtn.setOnAction(e -> output.appendText(controller.runCommand("close", fileField.getText()) + "\n"));
        searchBtn.setOnAction(e -> output.appendText(controller.runCommand("search", fileField.getText()) + "\n"));

        GridPane root = new GridPane();
        root.setPadding(new Insets(12));
        root.setVgap(8);
        root.setHgap(8);

        root.add(fileLabel, 0, 0);
        root.add(fileField, 1, 0, 3, 1);
        root.add(createBtn, 0, 1);
        root.add(openBtn, 1, 1);
        root.add(closeBtn, 2, 1);
        root.add(searchBtn, 3, 1);
        root.add(output, 0, 2, 4, 1);

        stage.setTitle("File Management System");
        stage.setScene(new Scene(root, 640, 340));
        stage.show();
    }

    public static void main(String[] args) {
        launch(args);
    }
}

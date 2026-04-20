package com.fms;

import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class FmsController {
    private static final String EXECUTABLE_PATH = "../cfs/fms";

    public String runCommand(String operation, String fileName) {
        if (fileName == null || fileName.isBlank()) {
            return "Please enter a file name.";
        }

        ProcessBuilder pb = new ProcessBuilder(EXECUTABLE_PATH, operation, fileName.trim());
        pb.redirectErrorStream(true);

        try {
            Process process = pb.start();
            StringBuilder output = new StringBuilder();

            try (BufferedReader br = new BufferedReader(new InputStreamReader(process.getInputStream()))) {
                String line;
                while ((line = br.readLine()) != null) {
                    output.append(line).append(System.lineSeparator());
                }
            }

            int exitCode = process.waitFor();
            if (exitCode != 0 && output.length() == 0) {
                return "Operation failed with exit code " + exitCode + ".";
            }
            return output.toString().trim();
        } catch (InterruptedException ex) {
            Thread.currentThread().interrupt();
            return "Error running C backend: " + ex.getMessage();
        } catch (IOException ex) {
            return "Error running C backend: " + ex.getMessage();
        }
    }
}

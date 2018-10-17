package pl.edu.agh.iisg.to.javafx.cw3.command;

public interface Command {

	void execute();

	void undo();

	void redo();

	String getName();
}

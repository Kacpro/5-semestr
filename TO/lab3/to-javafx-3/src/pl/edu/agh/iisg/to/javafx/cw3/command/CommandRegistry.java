package pl.edu.agh.iisg.to.javafx.cw3.command;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

public class CommandRegistry {

	private ObservableList<Command> commandStack = FXCollections
			.observableArrayList();

	private ObservableList<Command> undoStack = FXCollections.observableArrayList();

	public void executeCommand(Command command) {
		command.execute();
		commandStack.add(command);
		undoStack.removeAll();
	}

	public void redo()
	{
		if (!undoStack.isEmpty())
		{
			Command lastCommand = undoStack.remove(undoStack.size() - 1);
			lastCommand.redo();
			commandStack.add(lastCommand);
		}
	}

	public void undo()
	{
		if (!commandStack.isEmpty())
		{
			Command lastCommand = commandStack.remove(commandStack.size() - 1);
			lastCommand.undo();
			undoStack.add(lastCommand);
		}
	}

	public ObservableList<Command> getCommandStack() {
		return commandStack;
	}
}

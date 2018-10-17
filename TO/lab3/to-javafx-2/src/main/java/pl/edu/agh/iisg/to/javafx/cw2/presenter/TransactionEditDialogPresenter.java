package pl.edu.agh.iisg.to.javafx.cw2.presenter;

import java.math.BigDecimal;
import java.text.DecimalFormat;
import java.time.LocalDate;
import java.time.format.DateTimeFormatter;
import java.util.Locale;

import javafx.event.ActionEvent;
import javafx.fxml.FXML;
import javafx.scene.control.DatePicker;
import javafx.scene.control.TextField;
import javafx.stage.Stage;
import pl.edu.agh.iisg.to.javafx.cw2.model.Category;
import pl.edu.agh.iisg.to.javafx.cw2.model.Transaction;

public class TransactionEditDialogPresenter {

	private Transaction transaction;

	@FXML
	private TextField dateTextField;

	@FXML
	private TextField payeeTextField;

	@FXML
	private TextField categoryTextField;

	@FXML
	private TextField inflowTextField;
	
	private Stage dialogStage;
	
	private boolean approved;
	
	public void setDialogStage(Stage dialogStage) {
		this.dialogStage = dialogStage;
	}
	
	public void setData(Transaction transaction) {
		this.transaction = transaction;
		updateControls();
	}
	
	public boolean isApproved() {
		return approved;
	}
	
	@FXML
	private void handleOkAction(ActionEvent event) {
		if (isInputValid())
		{
			updateModel();
			dialogStage.close();
		}
	}
	
	@FXML
	private void handleCancelAction(ActionEvent event) {
		dialogStage.close();
	}
	
	private boolean isInputValid() {
		// TODO: implement
		return true;
	}
	
	private void updateModel() {
		transaction.setCategory(new Category(categoryTextField.getText()));
		transaction.setPayee(payeeTextField.getText());
	}
	
	private void updateControls() {
		inflowTextField.setText(transaction.getInflow().toString());
		categoryTextField.setText(transaction.getCategory().toString());
		payeeTextField.setText(transaction.getPayee().toString());
		dateTextField.setText(transaction.getDate().toString());
	}
}

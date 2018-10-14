package pl.edu.agh.iisg.to.javafx.cw1.model;

import javafx.collections.FXCollections;
import javafx.collections.ObservableList;

import java.math.BigInteger;

public class Account {

	private String name;
	
	private BigInteger balance;
	
	private ObservableList<Transaction> transactions;

	public Account(String name) {
		this.name = name;
		this.balance = BigInteger.ZERO;
		this.transactions = FXCollections.observableArrayList();
	}

	public final String getName() {
		return name;
	}
	
	
	public final BigInteger getBalance() {
		return balance;
	}
	
	public final ObservableList<Transaction> getTransactions() {
		return transactions;
	}
	
	public final void addTransaction(Transaction transaction) {
		this.transactions.add(transaction);
	}
		
}

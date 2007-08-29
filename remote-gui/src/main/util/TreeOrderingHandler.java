package util;
import java.util.ArrayList;
import java.util.Iterator;


public class TreeOrderingHandler {

	boolean unique = false;
	ArrayList orderings;

	public void clearCurrentOrderings()
	{
		orderings = new ArrayList();
		unique = false;
	}

	public void addOrdering(TableRowOrdering ordering)
	{
		if (isOrderingAllowed(ordering))
		{
			this.orderings.add(ordering);
			this.unique = ordering.isTotal();
		}
	}

	public TableRowOrdering[] getOrderings()
	{
		return (TableRowOrdering[])orderings.toArray(new TableRowOrdering[0]);
	}

	protected TableRowOrdering getOrdering(int index)
	{
		return (TableRowOrdering)orderings.get(index);
	}

	protected boolean isOrderingAllowed(TableRowOrdering ordering)
	{
		if (this.unique) return false;
		Iterator it = orderings.iterator();
		while (it.hasNext())
		{
			TableRowOrdering o = (TableRowOrdering)it.next();
			if (o==ordering)
			{
				return false;
			}
		}
		return true;
	}

	public TableRowOrdering[] getAllowedOrderings(TableRowOrdering[] orderingList)
	{
		if (unique)
		{
			return new TableRowOrdering[0];
		}
		else
		{
			ArrayList allowedOrderings = new ArrayList();
			for (int i=0; i<orderingList.length; i++)
			{

				if (this.isOrderingAllowed(orderingList[i]))
				{
					allowedOrderings.add(orderingList[i]);
				}
			}
			return (TableRowOrdering[])allowedOrderings.toArray(new TableRowOrdering[0]);
		}
	}

	public boolean isUnique()
	{
		return unique;
	}
}

namespace SupermercadoApp.Models;

public class Venda
{
    public Guid Id { get; } = Guid.NewGuid();
    public DateTime DataHora { get; } = DateTime.Now;
    public List<ItemVenda> Itens { get; } = new();
    public decimal Total => Itens.Sum(i => i.Subtotal);

    public void AdicionarItem(ItemVenda item)
    {
        Itens.Add(item);
    }
}

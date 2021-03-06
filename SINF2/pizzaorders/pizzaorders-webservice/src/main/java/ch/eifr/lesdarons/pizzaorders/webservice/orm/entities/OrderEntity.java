package ch.eifr.lesdarons.pizzaorders.webservice.orm.entities;

import ch.eifr.lesdarons.pizzaorders.webservice.skeleton.Address;
import ch.eifr.lesdarons.pizzaorders.webservice.skeleton.Order;

import javax.persistence.*;
import java.util.Date;
import java.util.Set;

@Entity
@Table(name = "orders")
public class OrderEntity implements Order {
    private long id = -1;
    private Date dateTime;
    private Address deliveryAddress;
    private Set<PizzaToOrderAssocEntity> pizzas;

    public OrderEntity() {
    }

    public void setId(long id) {
        this.id = id;
    }

    public void setDateTime(Date dateTime) {
        this.dateTime = dateTime;
    }

    public void setDeliveryAddress(Address deliveryAddress) {
        this.deliveryAddress = deliveryAddress;
    }

    public void setPizzas(Set<PizzaToOrderAssocEntity> pizzas) {
        this.pizzas = pizzas;
    }

    @Override
    @Id
    @GeneratedValue(strategy = GenerationType.AUTO)
    public long getId() {
        return id;
    }

    @Override
    @Column
    public Date getDateTime() {
        return dateTime;
    }

    @Override
    @ManyToOne(targetEntity = AddressEntity.class,
            cascade = CascadeType.REMOVE,
            fetch = FetchType.EAGER)
    public Address getDeliveryAddress() {
        return deliveryAddress;
    }


    @Override
    @MapsId("assocId")
    @OneToMany
    public Set<PizzaToOrderAssocEntity> getPizzas() {
        return pizzas;
    }

    @Override
    public String toString() {
        return "OrderEntity{" +
                "id=" + id +
                ", dateTime=" + dateTime +
                ", deliveryAddress=" + deliveryAddress +
                ", pizzaAssocs=" + pizzas +
                '}';
    }
}
